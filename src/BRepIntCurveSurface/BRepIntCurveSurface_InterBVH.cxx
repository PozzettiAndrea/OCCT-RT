// Created on: 2024-12-01
// Created by: Andrea Pozzetti (with Claude Code assistance)
// Copyright (c) 2024 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#include <BRepIntCurveSurface_InterBVH.hxx>

#include <BVH_Traverse.hxx>
#include <BVH_Tools.hxx>
#include <BVH_BinnedBuilder.hxx>
#include <BVH_LinearBuilder.hxx>
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <StdFail_NotDone.hxx>
#include <Standard_OutOfRange.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <Poly_Triangle.hxx>
#include <TopLoc_Location.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <Adaptor3d_Surface.hxx>
#include <gp_Vec.hxx>
#include <set>

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <atomic>
#include <unordered_map>
#include <cmath>
#include <limits>

#ifdef _OPENMP
#include <omp.h>
#endif

// Debug timing stats (thread-safe)
namespace {
  std::atomic<long long> g_nodeTestCount{0};
  std::atomic<long long> g_faceTestCount{0};
  std::atomic<long long> g_intersectorTimeNs{0};
  std::atomic<long long> g_newtonIterations{0};
  std::atomic<long long> g_newtonFailures{0};
  // Newton failure breakdown counters
  std::atomic<long long> g_newtonFailSingular{0};   // Singular Jacobian
  std::atomic<long long> g_newtonFailMaxIter{0};    // Max iterations exceeded
  std::atomic<long long> g_newtonFailResidual{0};   // Final residual too large

  //! Newton refinement result
  enum class NewtonResult {
    Converged,
    SingularJacobian,
    MaxIterations,
    ResidualTooLarge
  };

  // Default vertex welding tolerance
  constexpr double DEFAULT_WELD_TOLERANCE = 1.0e-3;

  //! Spatial hash grid for vertex welding
  //! Uses cell size = tolerance, checks 27 neighboring cells for proper tolerance matching
  class SpatialVertexGrid {
  public:
    SpatialVertexGrid(double tolerance, size_t reserveSize)
      : myTol(tolerance), myTolSq(tolerance * tolerance)
    {
      myCells.reserve(reserveSize);
    }

    //! Find existing vertex within tolerance, or add new one
    //! Returns vertex index
    Standard_Integer FindOrAdd(const BVH_Vec3d& v, std::vector<BVH_Vec3d>& vertices) {
      int64_t cx = CellCoord(v[0]);
      int64_t cy = CellCoord(v[1]);
      int64_t cz = CellCoord(v[2]);

      // Check all 27 neighboring cells (including center)
      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          for (int dz = -1; dz <= 1; ++dz) {
            uint64_t cellKey = MakeCellKey(cx + dx, cy + dy, cz + dz);
            auto it = myCells.find(cellKey);
            if (it != myCells.end()) {
              // Check all vertices in this cell
              for (Standard_Integer idx : it->second) {
                const BVH_Vec3d& existing = vertices[idx];
                double dx2 = v[0] - existing[0];
                double dy2 = v[1] - existing[1];
                double dz2 = v[2] - existing[2];
                if (dx2*dx2 + dy2*dy2 + dz2*dz2 < myTolSq) {
                  return idx;  // Found match
                }
              }
            }
          }
        }
      }

      // No match found - add new vertex
      Standard_Integer newIdx = static_cast<Standard_Integer>(vertices.size());
      vertices.push_back(v);

      // Add to grid cell
      uint64_t cellKey = MakeCellKey(cx, cy, cz);
      myCells[cellKey].push_back(newIdx);

      return newIdx;
    }

  private:
    int64_t CellCoord(double val) const {
      return static_cast<int64_t>(std::floor(val / myTol));
    }

    uint64_t MakeCellKey(int64_t x, int64_t y, int64_t z) const {
      // Combine 3 coordinates into single key
      // Use 21 bits per coordinate (covers ±1 million cells)
      return ((static_cast<uint64_t>(x) & 0x1FFFFF)) |
             ((static_cast<uint64_t>(y) & 0x1FFFFF) << 21) |
             ((static_cast<uint64_t>(z) & 0x1FFFFF) << 42);
    }

    double myTol;
    double myTolSq;
    std::unordered_map<uint64_t, std::vector<Standard_Integer>> myCells;
  };
}

//! Newton iteration to refine ray-surface intersection starting from approximate UV
//! Returns NewtonResult indicating convergence status
//! @param theSurface Surface adaptor
//! @param theRayOrigin Ray origin point
//! @param theRayDir Ray direction (normalized)
//! @param theU Initial U parameter (will be refined)
//! @param theV Initial V parameter (will be refined)
//! @param theT Output: parameter along ray
//! @param thePnt Output: intersection point
//! @param theIterCount Output: number of iterations performed (for stats, avoids atomics in hot loop)
//! @param theTol Convergence tolerance
//! @param theMaxIter Maximum iterations
static NewtonResult RefineIntersectionNewton(
  const Adaptor3d_Surface& theSurface,
  const gp_Pnt& theRayOrigin,
  const gp_Dir& theRayDir,
  Standard_Real& theU,
  Standard_Real& theV,
  Standard_Real& theT,
  gp_Pnt& thePnt,
  Standard_Integer& theIterCount,
  const Standard_Real theTol = 1e-7,
  const Standard_Integer theMaxIter = 10)
{
  // Newton iteration to solve: S(u,v) = O + t*D
  // where S is surface, O is ray origin, D is ray direction
  // We minimize |S(u,v) - (O + t*D)|^2
  //
  // Equivalent to solving:
  //   (S - O - t*D) . dS/du = 0
  //   (S - O - t*D) . dS/dv = 0
  //   (S - O - t*D) . D = 0

  Standard_Real u = theU;
  Standard_Real v = theV;

  // Get surface parameter bounds
  Standard_Real uMin = theSurface.FirstUParameter();
  Standard_Real uMax = theSurface.LastUParameter();
  Standard_Real vMin = theSurface.FirstVParameter();
  Standard_Real vMax = theSurface.LastVParameter();

  gp_Pnt S;
  gp_Vec dSu, dSv;
  gp_Vec D(theRayDir);
  gp_Pnt O = theRayOrigin;

  Standard_Boolean hitSingular = Standard_False;

  Standard_Integer localIterCount = 0;
  for (Standard_Integer iter = 0; iter < theMaxIter; ++iter)
  {
    ++localIterCount;

    // Evaluate surface and derivatives
    theSurface.D1(u, v, S, dSu, dSv);

    // Compute t = (S - O) . D (projection of surface point onto ray)
    gp_Vec SO(O, S);
    Standard_Real t = SO.Dot(D);

    // Residual vector: r = S - (O + t*D)
    gp_Pnt rayPt = O.Translated(t * D);
    gp_Vec r(rayPt, S);

    // Check convergence
    Standard_Real residual = r.Magnitude();
    if (residual < theTol)
    {
      theU = u;
      theV = v;
      theT = t;
      thePnt = S;
      theIterCount = localIterCount;
      return NewtonResult::Converged;
    }

    // Build 2x2 Jacobian for Newton step in (u,v)
    // J = [ dSu.dSu  dSu.dSv ]
    //     [ dSv.dSu  dSv.dSv ]
    //
    // RHS = -[ r.dSu ]
    //       [ r.dSv ]

    Standard_Real j11 = dSu.Dot(dSu);
    Standard_Real j12 = dSu.Dot(dSv);
    Standard_Real j21 = j12;  // symmetric
    Standard_Real j22 = dSv.Dot(dSv);

    Standard_Real b1 = -r.Dot(dSu);
    Standard_Real b2 = -r.Dot(dSv);

    // Solve 2x2 system: J * [du, dv]^T = [b1, b2]^T
    Standard_Real det = j11 * j22 - j12 * j21;
    if (std::abs(det) < 1e-15)
    {
      // Singular Jacobian - can't continue
      hitSingular = Standard_True;
      break;
    }

    Standard_Real du = (j22 * b1 - j12 * b2) / det;
    Standard_Real dv = (j11 * b2 - j21 * b1) / det;

    // Update with damping if step is too large
    Standard_Real stepSize = std::sqrt(du*du + dv*dv);
    if (stepSize > 0.5)
    {
      Standard_Real scale = 0.5 / stepSize;
      du *= scale;
      dv *= scale;
    }

    u += du;
    v += dv;

    // Clamp to parameter bounds
    u = std::max(uMin, std::min(uMax, u));
    v = std::max(vMin, std::min(vMax, v));
  }

  // Did not converge within max iterations, but return best result
  theSurface.D0(u, v, S);
  gp_Vec SO(O, S);
  theT = SO.Dot(D);
  theU = u;
  theV = v;
  thePnt = S;

  // Check if final result is acceptable
  gp_Pnt rayPt = O.Translated(theT * D);
  Standard_Real finalResidual = S.Distance(rayPt);

  theIterCount = localIterCount;

  if (finalResidual < theTol * 10)
  {
    // Close enough - accept as converged
    return NewtonResult::Converged;
  }
  else if (hitSingular)
  {
    return NewtonResult::SingularJacobian;
  }
  else
  {
    // Hit max iterations without converging
    return NewtonResult::MaxIterations;
  }
}

namespace
{
  //! Möller–Trumbore ray-triangle intersection
  //! Returns true if ray hits triangle, outputs t parameter and barycentric coords (u, v)
  inline Standard_Boolean RayTriangleIntersect(
    const BVH_Vec3d& rayOrigin,
    const BVH_Vec3d& rayDir,
    const BVH_Vec3d& v0,
    const BVH_Vec3d& v1,
    const BVH_Vec3d& v2,
    Standard_Real& t,
    Standard_Real& u,
    Standard_Real& v)
  {
    const Standard_Real EPSILON = 1e-12;

    BVH_Vec3d edge1 = v1 - v0;
    BVH_Vec3d edge2 = v2 - v0;

    BVH_Vec3d h;
    h[0] = rayDir[1] * edge2[2] - rayDir[2] * edge2[1];
    h[1] = rayDir[2] * edge2[0] - rayDir[0] * edge2[2];
    h[2] = rayDir[0] * edge2[1] - rayDir[1] * edge2[0];

    Standard_Real a = edge1[0] * h[0] + edge1[1] * h[1] + edge1[2] * h[2];

    if (a > -EPSILON && a < EPSILON)
      return Standard_False; // Ray parallel to triangle

    Standard_Real f = 1.0 / a;
    BVH_Vec3d s = rayOrigin - v0;

    u = f * (s[0] * h[0] + s[1] * h[1] + s[2] * h[2]);
    if (u < 0.0 || u > 1.0)
      return Standard_False;

    BVH_Vec3d q;
    q[0] = s[1] * edge1[2] - s[2] * edge1[1];
    q[1] = s[2] * edge1[0] - s[0] * edge1[2];
    q[2] = s[0] * edge1[1] - s[1] * edge1[0];

    v = f * (rayDir[0] * q[0] + rayDir[1] * q[1] + rayDir[2] * q[2]);
    if (v < 0.0 || u + v > 1.0)
      return Standard_False;

    t = f * (edge2[0] * q[0] + edge2[1] * q[1] + edge2[2] * q[2]);

    return t > EPSILON; // Hit if t is positive
  }

  //! Triangle BVH traverser - finds the closest triangle hit and returns the face index
  class BRepIntCurveSurface_TriangleTraverser
  {
  public:
    BRepIntCurveSurface_TriangleTraverser()
      : myTriBVH(nullptr),
        myTriangleInfo(nullptr),
        myClosestT(RealLast()),
        myHitTriangleIndex(-1),
        myHitFaceIndex(-1),
        myMinParam(0.0),
        myMaxParam(RealLast()),
        myNodeTestCount(0)
    {}

    void SetTriBVH(BRepIntCurveSurface_TriBVH* theBVH)
    {
      myTriBVH = theBVH;
    }

    void SetTriangleInfo(const std::vector<BRepIntCurveSurface_TriangleInfo>* theInfo)
    {
      myTriangleInfo = theInfo;
    }

    void SetRay(const gp_Lin& theRay, Standard_Real theMin, Standard_Real theMax)
    {
      myRayOrigin[0] = theRay.Location().X();
      myRayOrigin[1] = theRay.Location().Y();
      myRayOrigin[2] = theRay.Location().Z();
      myRayDir[0] = theRay.Direction().X();
      myRayDir[1] = theRay.Direction().Y();
      myRayDir[2] = theRay.Direction().Z();

      // Precompute inverse direction for fast ray-box intersection
      // Using a small epsilon to avoid division by zero
      const Standard_Real epsilon = 1e-12;
      for (int i = 0; i < 3; ++i)
      {
        myInvRayDir[i] = (std::abs(myRayDir[i]) > epsilon) ? (1.0 / myRayDir[i]) :
                         (myRayDir[i] >= 0 ? 1.0 / epsilon : -1.0 / epsilon);
      }

      myMinParam = theMin;
      myMaxParam = theMax;
      myClosestT = theMax;
      myHitTriangleIndex = -1;
      myHitFaceIndex = -1;
      myHitBaryU = 0.0;
      myHitBaryV = 0.0;
    }

    //! Traverse the triangle BVH to find closest hit
    void Select()
    {
      if (myTriBVH == nullptr || myTriBVH->BVH().IsNull())
        return;

      const opencascade::handle<BVH_Tree<Standard_Real, 3>>& aBVH = myTriBVH->BVH();

      // Stack-based traversal
      std::vector<Standard_Integer> aStack;
      aStack.reserve(64);
      aStack.push_back(0); // Start at root

      while (!aStack.empty())
      {
        Standard_Integer aNodeIdx = aStack.back();
        aStack.pop_back();

        if (aNodeIdx < 0)
          continue;

        // Get node bounds
        const BVH_Vec3d& aMinPt = aBVH->MinPoint(aNodeIdx);
        const BVH_Vec3d& aMaxPt = aBVH->MaxPoint(aNodeIdx);

        ++myNodeTestCount;  // Thread-local counter (no atomic overhead)

        // Test ray against box
        Standard_Real tNear, tFar;
        if (!RayBoxIntersect(aMinPt, aMaxPt, tNear, tFar))
          continue;

        // Skip if box is behind ray or past current best hit
        if (tFar < myMinParam || tNear > myClosestT)
          continue;

        if (aBVH->IsOuter(aNodeIdx))
        {
          // Leaf node - test triangles
          Standard_Integer aFirstTriIdx = aBVH->BegPrimitive(aNodeIdx);
          Standard_Integer aLastTriIdx = aBVH->EndPrimitive(aNodeIdx);

          for (Standard_Integer triIdx = aFirstTriIdx; triIdx <= aLastTriIdx; ++triIdx)
          {
            TestTriangle(triIdx);
          }
        }
        else
        {
          // Inner node - push children (closer child first for efficiency)
          Standard_Integer aLeftChild = aBVH->Child<0>(aNodeIdx);
          Standard_Integer aRightChild = aBVH->Child<1>(aNodeIdx);

          // Simple heuristic: push both, farther first (will be processed last)
          aStack.push_back(aRightChild);
          aStack.push_back(aLeftChild);
        }
      }
    }

    //! Get the hit face index (0-based), or -1 if no hit
    Standard_Integer GetHitFaceIndex() const { return myHitFaceIndex; }

    //! Get the parameter t on the ray
    Standard_Real GetHitT() const { return myClosestT; }

    //! Get barycentric coordinates of hit
    void GetHitBarycentric(Standard_Real& u, Standard_Real& v) const
    {
      u = myHitBaryU;
      v = myHitBaryV;
    }

    //! Get the triangle index that was hit
    Standard_Integer GetHitTriangleIndex() const { return myHitTriangleIndex; }

    //! Get the number of BVH node tests performed (thread-local, no atomic overhead)
    Standard_Integer GetNodeTestCount() const { return myNodeTestCount; }

  private:
    //! Ray-box intersection test using precomputed inverse direction (slab method)
    //! This is the optimized version that avoids per-test division
    Standard_Boolean RayBoxIntersect(const BVH_Vec3d& boxMin, const BVH_Vec3d& boxMax,
                                      Standard_Real& tNear, Standard_Real& tFar) const
    {
      // Optimized slab method using precomputed inverse direction
      Standard_Real t1 = (boxMin[0] - myRayOrigin[0]) * myInvRayDir[0];
      Standard_Real t2 = (boxMax[0] - myRayOrigin[0]) * myInvRayDir[0];

      tNear = std::min(t1, t2);
      tFar = std::max(t1, t2);

      t1 = (boxMin[1] - myRayOrigin[1]) * myInvRayDir[1];
      t2 = (boxMax[1] - myRayOrigin[1]) * myInvRayDir[1];

      tNear = std::max(tNear, std::min(t1, t2));
      tFar = std::min(tFar, std::max(t1, t2));

      t1 = (boxMin[2] - myRayOrigin[2]) * myInvRayDir[2];
      t2 = (boxMax[2] - myRayOrigin[2]) * myInvRayDir[2];

      tNear = std::max(tNear, std::min(t1, t2));
      tFar = std::min(tFar, std::max(t1, t2));

      return tNear <= tFar;
    }

    //! Test a single triangle (triIdx is the BVH primitive index after reordering)
    void TestTriangle(Standard_Integer triIdx)
    {
      if (myTriangleInfo == nullptr)
        return;

      // Validate triIdx against Elements array
      if (triIdx < 0 || triIdx >= static_cast<Standard_Integer>(myTriBVH->Elements.size()))
        return;

      // Get triangle data from BVH - elem contains:
      // x, y, z = vertex indices, w = ORIGINAL triangle index (preserved through Swap)
      const BVH_Vec4i& elem = myTriBVH->Elements[triIdx];

      // Validate vertex indices
      if (elem[0] < 0 || elem[0] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()) ||
          elem[1] < 0 || elem[1] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()) ||
          elem[2] < 0 || elem[2] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()))
        return;

      // Get original triangle index from 4th component
      Standard_Integer originalTriIdx = elem[3];
      if (originalTriIdx < 0 || originalTriIdx >= static_cast<Standard_Integer>(myTriangleInfo->size()))
        return;

      const BVH_Vec3d& v0 = myTriBVH->Vertices[elem[0]];
      const BVH_Vec3d& v1 = myTriBVH->Vertices[elem[1]];
      const BVH_Vec3d& v2 = myTriBVH->Vertices[elem[2]];

      Standard_Real t, u, v;
      if (RayTriangleIntersect(myRayOrigin, myRayDir, v0, v1, v2, t, u, v))
      {
        if (t >= myMinParam && t < myClosestT)
        {
          myClosestT = t;
          myHitTriangleIndex = originalTriIdx;  // Use ORIGINAL index for myTriangleInfo lookup
          myHitFaceIndex = (*myTriangleInfo)[originalTriIdx].FaceIndex;
          myHitBaryU = u;
          myHitBaryV = v;
        }
      }
    }

    BRepIntCurveSurface_TriBVH* myTriBVH;
    const std::vector<BRepIntCurveSurface_TriangleInfo>* myTriangleInfo;
    BVH_Vec3d myRayOrigin;
    BVH_Vec3d myRayDir;
    BVH_Vec3d myInvRayDir;  // Precomputed 1/direction for fast ray-box tests
    Standard_Real myClosestT;
    Standard_Integer myHitTriangleIndex;
    Standard_Integer myHitFaceIndex;
    Standard_Real myMinParam;
    Standard_Real myMaxParam;
    Standard_Real myHitBaryU;
    Standard_Real myHitBaryV;
    Standard_Integer myNodeTestCount;  // Thread-local counter (no atomic overhead)
  };

  //! Triangle BVH traverser that counts ALL intersections (not just closest)
  //! Used for PerformBatchCount with tessellation acceleration
  class BRepIntCurveSurface_TriangleCountTraverser
  {
  public:
    BRepIntCurveSurface_TriangleCountTraverser()
      : myTriBVH(nullptr),
        myTriangleInfo(nullptr),
        myHitCount(0),
        myMinParam(0.0),
        myMaxParam(RealLast()),
        myNodeTestCount(0)
    {}

    void SetTriBVH(BRepIntCurveSurface_TriBVH* theBVH)
    {
      myTriBVH = theBVH;
    }

    void SetTriangleInfo(const std::vector<BRepIntCurveSurface_TriangleInfo>* theInfo)
    {
      myTriangleInfo = theInfo;
    }

    void SetRay(const gp_Lin& theRay, Standard_Real theMin, Standard_Real theMax)
    {
      myRayOrigin[0] = theRay.Location().X();
      myRayOrigin[1] = theRay.Location().Y();
      myRayOrigin[2] = theRay.Location().Z();
      myRayDir[0] = theRay.Direction().X();
      myRayDir[1] = theRay.Direction().Y();
      myRayDir[2] = theRay.Direction().Z();

      // Precompute inverse direction for fast ray-box intersection
      const Standard_Real epsilon = 1e-12;
      for (int i = 0; i < 3; ++i)
      {
        myInvRayDir[i] = (std::abs(myRayDir[i]) > epsilon) ? (1.0 / myRayDir[i]) :
                         (myRayDir[i] >= 0 ? 1.0 / epsilon : -1.0 / epsilon);
      }

      myMinParam = theMin;
      myMaxParam = theMax;
      myHitCount = 0;
    }

    //! Traverse the triangle BVH to count ALL hits
    void Select()
    {
      if (myTriBVH == nullptr || myTriBVH->BVH().IsNull())
        return;

      const opencascade::handle<BVH_Tree<Standard_Real, 3>>& aBVH = myTriBVH->BVH();

      // Stack-based traversal
      std::vector<Standard_Integer> aStack;
      aStack.reserve(64);
      aStack.push_back(0); // Start at root

      while (!aStack.empty())
      {
        Standard_Integer aNodeIdx = aStack.back();
        aStack.pop_back();

        if (aNodeIdx < 0)
          continue;

        // Get node bounds
        const BVH_Vec3d& aMinPt = aBVH->MinPoint(aNodeIdx);
        const BVH_Vec3d& aMaxPt = aBVH->MaxPoint(aNodeIdx);

        ++myNodeTestCount;  // Thread-local counter (no atomic overhead)

        // Test ray against box
        Standard_Real tNear, tFar;
        if (!RayBoxIntersect(aMinPt, aMaxPt, tNear, tFar))
          continue;

        // Skip if box is entirely behind ray or past max param
        if (tFar < myMinParam || tNear > myMaxParam)
          continue;

        if (aBVH->IsOuter(aNodeIdx))
        {
          // Leaf node - test triangles
          Standard_Integer aFirstTriIdx = aBVH->BegPrimitive(aNodeIdx);
          Standard_Integer aLastTriIdx = aBVH->EndPrimitive(aNodeIdx);

          for (Standard_Integer triIdx = aFirstTriIdx; triIdx <= aLastTriIdx; ++triIdx)
          {
            TestTriangle(triIdx);
          }
        }
        else
        {
          // Inner node - push children
          Standard_Integer aLeftChild = aBVH->Child<0>(aNodeIdx);
          Standard_Integer aRightChild = aBVH->Child<1>(aNodeIdx);

          aStack.push_back(aRightChild);
          aStack.push_back(aLeftChild);
        }
      }
    }

    //! Get the total number of hits
    Standard_Integer GetHitCount() const { return myHitCount; }

    //! Get the number of BVH node tests performed (thread-local, no atomic overhead)
    Standard_Integer GetNodeTestCount() const { return myNodeTestCount; }

  private:
    //! Ray-box intersection test using precomputed inverse direction (slab method)
    Standard_Boolean RayBoxIntersect(const BVH_Vec3d& boxMin, const BVH_Vec3d& boxMax,
                                      Standard_Real& tNear, Standard_Real& tFar) const
    {
      Standard_Real t1 = (boxMin[0] - myRayOrigin[0]) * myInvRayDir[0];
      Standard_Real t2 = (boxMax[0] - myRayOrigin[0]) * myInvRayDir[0];

      tNear = std::min(t1, t2);
      tFar = std::max(t1, t2);

      t1 = (boxMin[1] - myRayOrigin[1]) * myInvRayDir[1];
      t2 = (boxMax[1] - myRayOrigin[1]) * myInvRayDir[1];

      tNear = std::max(tNear, std::min(t1, t2));
      tFar = std::min(tFar, std::max(t1, t2));

      t1 = (boxMin[2] - myRayOrigin[2]) * myInvRayDir[2];
      t2 = (boxMax[2] - myRayOrigin[2]) * myInvRayDir[2];

      tNear = std::max(tNear, std::min(t1, t2));
      tFar = std::min(tFar, std::max(t1, t2));

      return tNear <= tFar;
    }

    //! Test a single triangle and count if hit
    void TestTriangle(Standard_Integer triIdx)
    {
      if (myTriangleInfo == nullptr)
        return;

      if (triIdx < 0 || triIdx >= static_cast<Standard_Integer>(myTriBVH->Elements.size()))
        return;

      const BVH_Vec4i& elem = myTriBVH->Elements[triIdx];

      if (elem[0] < 0 || elem[0] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()) ||
          elem[1] < 0 || elem[1] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()) ||
          elem[2] < 0 || elem[2] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()))
        return;

      const BVH_Vec3d& v0 = myTriBVH->Vertices[elem[0]];
      const BVH_Vec3d& v1 = myTriBVH->Vertices[elem[1]];
      const BVH_Vec3d& v2 = myTriBVH->Vertices[elem[2]];

      Standard_Real t, u, v;
      if (RayTriangleIntersect(myRayOrigin, myRayDir, v0, v1, v2, t, u, v))
      {
        if (t >= myMinParam && t <= myMaxParam)
        {
          ++myHitCount;
        }
      }
    }

    BRepIntCurveSurface_TriBVH* myTriBVH;
    const std::vector<BRepIntCurveSurface_TriangleInfo>* myTriangleInfo;
    BVH_Vec3d myRayOrigin;
    BVH_Vec3d myRayDir;
    BVH_Vec3d myInvRayDir;
    Standard_Integer myHitCount;
    Standard_Real myMinParam;
    Standard_Real myMaxParam;
    Standard_Integer myNodeTestCount;  // Thread-local counter (no atomic overhead)
  };
}

//=================================================================================================

BRepIntCurveSurface_InterBVH::BRepIntCurveSurface_InterBVH()
  : myTolerance(Precision::Confusion()),
    myDeflection(0.0),
    myUseTessellation(Standard_False),
    myIsLoaded(Standard_False),
    myIsDone(Standard_False),
    myNbPnt(0),
    myBackend(BRepIntCurveSurface_BVHBackend::OCCT_BVH),  // Default to fastest single-ray
    myUseOpenMP(Standard_True)  // Enable OpenMP by default
#ifdef OCCT_USE_EMBREE
    , myEmbreeDevice(nullptr)
    , myEmbreeScene(nullptr)
#endif
{
}

//=================================================================================================

BRepIntCurveSurface_InterBVH::~BRepIntCurveSurface_InterBVH()
{
#ifdef OCCT_USE_EMBREE
  if (myEmbreeScene) {
    rtcReleaseScene(myEmbreeScene);
    myEmbreeScene = nullptr;
  }
  if (myEmbreeDevice) {
    rtcReleaseDevice(myEmbreeDevice);
    myEmbreeDevice = nullptr;
  }
#endif
}

//=================================================================================================
// SIMD helpers for Embree batch intersection
//=================================================================================================

#ifdef OCCT_USE_EMBREE
namespace {
  //! Process 4 rays at once using SSE (rtcIntersect4)
  //! @param theScene Embree scene
  //! @param theRays Array of 4 rays (input)
  //! @param theTriIdx Output: triangle indices (-1 if miss)
  //! @param theHitT Output: hit distances
  //! @param theBaryU Output: barycentric U coordinates
  //! @param theBaryV Output: barycentric V coordinates
  void IntersectEmbree4(
      RTCScene theScene,
      const gp_Lin* theRays,
      Standard_Integer* theTriIdx,
      Standard_Real* theHitT,
      Standard_Real* theBaryU,
      Standard_Real* theBaryV)
  {
    alignas(16) RTCRayHit4 rayhit4;
    alignas(16) int valid[4] = {-1, -1, -1, -1};

    // Setup 4 rays
    for (int i = 0; i < 4; ++i) {
      rayhit4.ray.org_x[i] = static_cast<float>(theRays[i].Location().X());
      rayhit4.ray.org_y[i] = static_cast<float>(theRays[i].Location().Y());
      rayhit4.ray.org_z[i] = static_cast<float>(theRays[i].Location().Z());
      rayhit4.ray.dir_x[i] = static_cast<float>(theRays[i].Direction().X());
      rayhit4.ray.dir_y[i] = static_cast<float>(theRays[i].Direction().Y());
      rayhit4.ray.dir_z[i] = static_cast<float>(theRays[i].Direction().Z());
      rayhit4.ray.tnear[i] = 0.0f;
      rayhit4.ray.tfar[i] = std::numeric_limits<float>::infinity();
      rayhit4.ray.mask[i] = static_cast<unsigned int>(-1);
      rayhit4.ray.flags[i] = 0;
      rayhit4.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
      rayhit4.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
    }

    RTCIntersectContext ctx;
    rtcInitIntersectContext(&ctx);
    rtcIntersect4(valid, theScene, &ctx, &rayhit4);

    // Extract results
    for (int i = 0; i < 4; ++i) {
      if (rayhit4.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
        theTriIdx[i] = static_cast<Standard_Integer>(rayhit4.hit.primID[i]);
        theHitT[i] = static_cast<Standard_Real>(rayhit4.ray.tfar[i]);
        theBaryU[i] = static_cast<Standard_Real>(rayhit4.hit.u[i]);
        theBaryV[i] = static_cast<Standard_Real>(rayhit4.hit.v[i]);
      } else {
        theTriIdx[i] = -1;
        theHitT[i] = -1.0;
        theBaryU[i] = 0.0;
        theBaryV[i] = 0.0;
      }
    }
  }

  //! Process 8 rays at once using AVX (rtcIntersect8)
  //! @param theScene Embree scene
  //! @param theRays Array of 8 rays (input)
  //! @param theTriIdx Output: triangle indices (-1 if miss)
  //! @param theHitT Output: hit distances
  //! @param theBaryU Output: barycentric U coordinates
  //! @param theBaryV Output: barycentric V coordinates
  void IntersectEmbree8(
      RTCScene theScene,
      const gp_Lin* theRays,
      Standard_Integer* theTriIdx,
      Standard_Real* theHitT,
      Standard_Real* theBaryU,
      Standard_Real* theBaryV)
  {
    alignas(32) RTCRayHit8 rayhit8;
    alignas(32) int valid[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

    // Setup 8 rays
    for (int i = 0; i < 8; ++i) {
      rayhit8.ray.org_x[i] = static_cast<float>(theRays[i].Location().X());
      rayhit8.ray.org_y[i] = static_cast<float>(theRays[i].Location().Y());
      rayhit8.ray.org_z[i] = static_cast<float>(theRays[i].Location().Z());
      rayhit8.ray.dir_x[i] = static_cast<float>(theRays[i].Direction().X());
      rayhit8.ray.dir_y[i] = static_cast<float>(theRays[i].Direction().Y());
      rayhit8.ray.dir_z[i] = static_cast<float>(theRays[i].Direction().Z());
      rayhit8.ray.tnear[i] = 0.0f;
      rayhit8.ray.tfar[i] = std::numeric_limits<float>::infinity();
      rayhit8.ray.mask[i] = static_cast<unsigned int>(-1);
      rayhit8.ray.flags[i] = 0;
      rayhit8.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
      rayhit8.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
    }

    RTCIntersectContext ctx;
    rtcInitIntersectContext(&ctx);
    rtcIntersect8(valid, theScene, &ctx, &rayhit8);

    // Extract results
    for (int i = 0; i < 8; ++i) {
      if (rayhit8.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID) {
        theTriIdx[i] = static_cast<Standard_Integer>(rayhit8.hit.primID[i]);
        theHitT[i] = static_cast<Standard_Real>(rayhit8.ray.tfar[i]);
        theBaryU[i] = static_cast<Standard_Real>(rayhit8.hit.u[i]);
        theBaryV[i] = static_cast<Standard_Real>(rayhit8.hit.v[i]);
      } else {
        theTriIdx[i] = -1;
        theHitT[i] = -1.0;
        theBaryU[i] = 0.0;
        theBaryV[i] = 0.0;
      }
    }
  }

  //! Single ray intersection using Embree (rtcIntersect1)
  void IntersectEmbree1(
      RTCScene theScene,
      const gp_Lin& theRay,
      Standard_Integer& theTriIdx,
      Standard_Real& theHitT,
      Standard_Real& theBaryU,
      Standard_Real& theBaryV)
  {
    RTCRayHit rayhit;
    rayhit.ray.org_x = static_cast<float>(theRay.Location().X());
    rayhit.ray.org_y = static_cast<float>(theRay.Location().Y());
    rayhit.ray.org_z = static_cast<float>(theRay.Location().Z());
    rayhit.ray.dir_x = static_cast<float>(theRay.Direction().X());
    rayhit.ray.dir_y = static_cast<float>(theRay.Direction().Y());
    rayhit.ray.dir_z = static_cast<float>(theRay.Direction().Z());
    rayhit.ray.tnear = 0.0f;
    rayhit.ray.tfar = std::numeric_limits<float>::infinity();
    rayhit.ray.mask = static_cast<unsigned int>(-1);
    rayhit.ray.flags = 0;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    rayhit.hit.primID = RTC_INVALID_GEOMETRY_ID;

    RTCIntersectContext ctx;
    rtcInitIntersectContext(&ctx);
    rtcIntersect1(theScene, &ctx, &rayhit);

    if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
      theTriIdx = static_cast<Standard_Integer>(rayhit.hit.primID);
      theHitT = static_cast<Standard_Real>(rayhit.ray.tfar);
      theBaryU = static_cast<Standard_Real>(rayhit.hit.u);
      theBaryV = static_cast<Standard_Real>(rayhit.hit.v);
    } else {
      theTriIdx = -1;
      theHitT = -1.0;
      theBaryU = 0.0;
      theBaryV = 0.0;
    }
  }
}
#endif

//=================================================================================================

void BRepIntCurveSurface_InterBVH::Load(const TopoDS_Shape& theShape,
                                         const Standard_Real theTol,
                                         const Standard_Real theDeflection)
{
  myIsLoaded = Standard_False;
  myIsDone = Standard_False;
  myNbPnt = 0;
  myResults.clear();
  myFaces.Clear();
  mySurfaceAdaptors.clear();
  myTriBVH.Nullify();
  myTriangleInfo.clear();
  myUseTessellation = Standard_False;

  myTolerance = theTol;
  // Always use tessellation - default to 0.1 if not specified
  myDeflection = (theDeflection > 0.0) ? theDeflection : 0.1;

  // Collect all faces from the shape
  TopExp_Explorer anExp(theShape, TopAbs_FACE);
  for (; anExp.More(); anExp.Next())
  {
    myFaces.Add(anExp.Current());
  }

  if (myFaces.IsEmpty())
    return;

  // Create surface adaptors for all faces (needed for Newton refinement)
  mySurfaceAdaptors.resize(myFaces.Extent());
  for (Standard_Integer i = 1; i <= myFaces.Extent(); ++i)
  {
    const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(i));
    mySurfaceAdaptors[i - 1] = new BRepAdaptor_Surface(aFace, Standard_True);
  }

  // Build triangle BVH for tessellation-accelerated intersection
  // NOTE: The shape must already be tessellated before calling Load().
  // Call BRepMesh_IncrementalMesh on the shape before Load() if needed.
  {
    std::cout << "  Building tessellation BVH (using existing triangulation)..." << std::endl;

    // Count total triangles first
    Standard_Integer totalTriangles = 0;
    for (Standard_Integer faceIdx = 1; faceIdx <= myFaces.Extent(); ++faceIdx)
    {
      const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(faceIdx));
      TopLoc_Location aLoc;
      Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(aFace, aLoc);
      if (!aTriangulation.IsNull())
      {
        totalTriangles += aTriangulation->NbTriangles();
      }
    }

    if (totalTriangles > 0)
    {
      // Create triangle BVH
      opencascade::handle<BVH_LinearBuilder<Standard_Real, 3>> aTriBuilder =
        new BVH_LinearBuilder<Standard_Real, 3>(4, 32);
      myTriBVH = new BRepIntCurveSurface_TriBVH(aTriBuilder);

      // Reserve space for vertices and triangles
      myTriangleInfo.reserve(totalTriangles);

      // Collect all triangles from all faces with vertex welding
      // Use spatial grid to merge duplicate vertices at face boundaries
      // Weld tolerance: use deflection or default 1e-3
      const double weldTol = std::max(myDeflection * 0.1, DEFAULT_WELD_TOLERANCE);
      std::cout << "  Weld tolerance: " << weldTol << std::endl;

      std::vector<BVH_Vec3d> uniqueVertices;
      uniqueVertices.reserve(totalTriangles * 2);
      std::vector<Standard_Integer> triangleIndices;
      triangleIndices.reserve(totalTriangles * 3);

      // Spatial grid for vertex welding (checks 27 neighboring cells for proper tolerance matching)
      SpatialVertexGrid vertexGrid(weldTol, totalTriangles * 2);

      // Helper lambda to get or create vertex index (welds duplicates)
      auto getVertexIndex = [&](const gp_Pnt& p) -> Standard_Integer {
        BVH_Vec3d v(p.X(), p.Y(), p.Z());
        return vertexGrid.FindOrAdd(v, uniqueVertices);
      };

      for (Standard_Integer faceIdx = 1; faceIdx <= myFaces.Extent(); ++faceIdx)
      {
        const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(faceIdx));
        TopLoc_Location aLoc;
        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(aFace, aLoc);

        if (aTriangulation.IsNull())
          continue;

        const gp_Trsf& aTrsf = aLoc.Transformation();
        Standard_Boolean hasTransform = (aLoc.IsIdentity() == Standard_False);

        // Get the UV nodes if available
        Standard_Boolean hasUVNodes = aTriangulation->HasUVNodes();

        for (Standard_Integer triIdx = 1; triIdx <= aTriangulation->NbTriangles(); ++triIdx)
        {
          const Poly_Triangle& aTri = aTriangulation->Triangle(triIdx);
          Standard_Integer n1, n2, n3;
          aTri.Get(n1, n2, n3);

          // Get 3D vertices
          gp_Pnt p1 = aTriangulation->Node(n1);
          gp_Pnt p2 = aTriangulation->Node(n2);
          gp_Pnt p3 = aTriangulation->Node(n3);

          // Apply face transformation if needed
          if (hasTransform)
          {
            p1.Transform(aTrsf);
            p2.Transform(aTrsf);
            p3.Transform(aTrsf);
          }

          // Get welded vertex indices for this triangle
          triangleIndices.push_back(getVertexIndex(p1));
          triangleIndices.push_back(getVertexIndex(p2));
          triangleIndices.push_back(getVertexIndex(p3));

          // Store triangle info with UV coordinates
          BRepIntCurveSurface_TriangleInfo aTriInfo;
          aTriInfo.FaceIndex = faceIdx - 1;  // 0-based

          if (hasUVNodes)
          {
            aTriInfo.UV0 = aTriangulation->UVNode(n1);
            aTriInfo.UV1 = aTriangulation->UVNode(n2);
            aTriInfo.UV2 = aTriangulation->UVNode(n3);
          }
          else
          {
            // No UV nodes - will need to use Face intersector anyway
            aTriInfo.UV0 = gp_Pnt2d(0, 0);
            aTriInfo.UV1 = gp_Pnt2d(0, 0);
            aTriInfo.UV2 = gp_Pnt2d(0, 0);
          }

          myTriangleInfo.push_back(aTriInfo);
        }
      }

      // Build the triangle BVH
      // BVH_Triangulation expects vertices as array and elements as triangle indices
      Standard_Integer nVertices = static_cast<Standard_Integer>(uniqueVertices.size());
      Standard_Integer nTriangles = static_cast<Standard_Integer>(myTriangleInfo.size());
      Standard_Integer nRawVertices = totalTriangles * 3;

      // Report vertex welding stats
      std::cout << "  Vertex welding: " << nRawVertices << " -> " << nVertices
                << " (" << std::fixed << std::setprecision(1)
                << (100.0 * nVertices / nRawVertices) << "% unique)" << std::endl;

      // Set welded vertices
      myTriBVH->Vertices.resize(nVertices);
      for (Standard_Integer i = 0; i < nVertices; ++i)
      {
        myTriBVH->Vertices[i] = uniqueVertices[i];
      }

      // Set triangle indices using welded vertex indices
      // IMPORTANT: Store original triangle index in 4th component (w) because
      // BVH building reorders Elements via Swap(), but myTriangleInfo stays in original order
      myTriBVH->Elements.resize(nTriangles);
      for (Standard_Integer i = 0; i < nTriangles; ++i)
      {
        myTriBVH->Elements[i] = BVH_Vec4i(
          triangleIndices[i * 3 + 0],
          triangleIndices[i * 3 + 1],
          triangleIndices[i * 3 + 2],
          i);  // w = original triangle index
      }

      // CRITICAL: Mark as dirty so BVH() will actually build the tree
      myTriBVH->MarkDirty();

      // Build the BVH
      myTriBVH->BVH();

      myUseTessellation = Standard_True;

      std::cout << "  Triangle BVH built: " << nTriangles << " triangles from "
                << myFaces.Extent() << " faces" << std::endl;

      // Debug: verify BVH structure
      if (!myTriBVH->BVH().IsNull())
      {
        std::cout << "  [DEBUG] BVH tree depth: " << myTriBVH->BVH()->Depth() << std::endl;
        std::cout << "  [DEBUG] BVH tree nodes: " << myTriBVH->BVH()->Length() << std::endl;
        std::cout << "  [DEBUG] Vertices array size: " << myTriBVH->Vertices.size() << std::endl;
        std::cout << "  [DEBUG] Elements array size: " << myTriBVH->Elements.size() << std::endl;
        std::cout << "  [DEBUG] TriangleInfo array size: " << myTriangleInfo.size() << std::endl;
        // Sample a few elements to verify w component
        if (nTriangles > 0)
        {
          std::cout << "  [DEBUG] Elements[0]: v=(" << myTriBVH->Elements[0][0] << ","
                    << myTriBVH->Elements[0][1] << "," << myTriBVH->Elements[0][2]
                    << ") origIdx=" << myTriBVH->Elements[0][3] << std::endl;
          if (nTriangles > 100)
          {
            std::cout << "  [DEBUG] Elements[100]: v=(" << myTriBVH->Elements[100][0] << ","
                      << myTriBVH->Elements[100][1] << "," << myTriBVH->Elements[100][2]
                      << ") origIdx=" << myTriBVH->Elements[100][3] << std::endl;
          }
        }
      }

#ifdef OCCT_USE_EMBREE
      // Build Embree scene for hardware-accelerated BVH traversal
      std::cout << "  Building Embree scene..." << std::endl;

      // Create Embree device if not already created
      if (!myEmbreeDevice) {
        myEmbreeDevice = rtcNewDevice(nullptr);
        if (!myEmbreeDevice) {
          std::cerr << "  ERROR: Failed to create Embree device!" << std::endl;
        }
      }

      // Release old scene if exists
      if (myEmbreeScene) {
        rtcReleaseScene(myEmbreeScene);
        myEmbreeScene = nullptr;
      }

      if (myEmbreeDevice) {
        myEmbreeScene = rtcNewScene(myEmbreeDevice);

        // Create triangle geometry
        RTCGeometry geom = rtcNewGeometry(myEmbreeDevice, RTC_GEOMETRY_TYPE_TRIANGLE);

        // Set vertex buffer (Embree expects float, not double)
        float* verts = (float*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_VERTEX,
            0, RTC_FORMAT_FLOAT3, 3 * sizeof(float), uniqueVertices.size());
        for (size_t i = 0; i < uniqueVertices.size(); ++i) {
          verts[3 * i + 0] = static_cast<float>(uniqueVertices[i][0]);
          verts[3 * i + 1] = static_cast<float>(uniqueVertices[i][1]);
          verts[3 * i + 2] = static_cast<float>(uniqueVertices[i][2]);
        }

        // Set index buffer
        unsigned* idxs = (unsigned*)rtcSetNewGeometryBuffer(geom, RTC_BUFFER_TYPE_INDEX,
            0, RTC_FORMAT_UINT3, 3 * sizeof(unsigned), nTriangles);
        for (Standard_Integer i = 0; i < nTriangles; ++i) {
          idxs[3 * i + 0] = static_cast<unsigned>(triangleIndices[i * 3 + 0]);
          idxs[3 * i + 1] = static_cast<unsigned>(triangleIndices[i * 3 + 1]);
          idxs[3 * i + 2] = static_cast<unsigned>(triangleIndices[i * 3 + 2]);
        }

        rtcCommitGeometry(geom);
        rtcAttachGeometry(myEmbreeScene, geom);
        rtcReleaseGeometry(geom);
        rtcCommitScene(myEmbreeScene);

        std::cout << "  Embree scene built: " << nTriangles << " triangles, "
                  << uniqueVertices.size() << " vertices" << std::endl;
      }
#endif
    }
  }

  myIsLoaded = Standard_True;
}

//=================================================================================================

void BRepIntCurveSurface_InterBVH::Perform(const gp_Lin& theLine,
                                            const Standard_Real theMin,
                                            const Standard_Real theMax)
{
  myIsDone = Standard_False;
  myNbPnt = 0;
  myResults.clear();

  if (!myIsLoaded || !myUseTessellation || myTriBVH.IsNull())
    return;

  // Use tessellation-accelerated path (same as PerformBatch for single ray)
  // Step 1: Fast triangle BVH traversal to find candidate face
  BRepIntCurveSurface_TriangleTraverser aTriTraverser;
  aTriTraverser.SetTriBVH(myTriBVH.get());
  aTriTraverser.SetTriangleInfo(&myTriangleInfo);
  aTriTraverser.SetRay(theLine, theMin, theMax);
  aTriTraverser.Select();

  Standard_Integer hitFaceIdx = aTriTraverser.GetHitFaceIndex();
  Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();

  if (hitFaceIdx >= 0 && hitFaceIdx < static_cast<Standard_Integer>(mySurfaceAdaptors.size()) &&
      hitTriIdx >= 0 && hitTriIdx < static_cast<Standard_Integer>(myTriangleInfo.size()))
  {
    // Step 2: UV-guided Newton refinement
    const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[hitTriIdx];
    Standard_Real baryU, baryV;
    aTriTraverser.GetHitBarycentric(baryU, baryV);
    Standard_Real baryW = 1.0 - baryU - baryV;

    // Interpolate UV: UV = w*UV0 + u*UV1 + v*UV2
    Standard_Real initU = baryW * triInfo.UV0.X() + baryU * triInfo.UV1.X() + baryV * triInfo.UV2.X();
    Standard_Real initV = baryW * triInfo.UV0.Y() + baryU * triInfo.UV1.Y() + baryV * triInfo.UV2.Y();

    // Refine using Newton iteration
    const BRepAdaptor_Surface& aSurface = *mySurfaceAdaptors[hitFaceIdx];
    Standard_Real finalU = initU;
    Standard_Real finalV = initV;
    Standard_Real finalT = 0.0;
    gp_Pnt finalPnt;
    Standard_Integer iterCount = 0;

    NewtonResult newtonResult = RefineIntersectionNewton(
      aSurface,
      theLine.Location(),
      theLine.Direction(),
      finalU, finalV, finalT, finalPnt, iterCount,
      myTolerance, 100);

    Standard_Real hitT = aTriTraverser.GetHitT();
    if (hitT >= theMin && hitT <= theMax)
    {
      BRepIntCurveSurface_HitResult aResult;
      aResult.IsValid = Standard_True;

      if (newtonResult == NewtonResult::Converged && finalT >= theMin && finalT <= theMax)
      {
        aResult.Point = finalPnt;
        aResult.U = finalU;
        aResult.V = finalV;
        aResult.W = finalT;
      }
      else
      {
        // Newton failed - use triangle intersection point directly
        aResult.Point = theLine.Location().Translated(hitT * gp_Vec(theLine.Direction()));
        aResult.U = initU;
        aResult.V = initV;
        aResult.W = hitT;
      }

      aResult.FaceIndex = hitFaceIdx + 1;  // 1-based
      aResult.Transition = IntCurveSurface_In;
      aResult.State = TopAbs_IN;

      // Compute surface normal and curvatures
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv;
      aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
      gp_Vec normalVec = dSdu.Crossed(dSdv);
      Standard_Real normalMag = normalVec.Magnitude();

      if (normalMag > 1e-10)
      {
        normalVec.Normalize();
        const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
        if (aFace.Orientation() == TopAbs_REVERSED)
          normalVec.Reverse();
        aResult.Normal = gp_Dir(normalVec);

        // Curvature computation
        Standard_Real E = dSdu.Dot(dSdu);
        Standard_Real F = dSdu.Dot(dSdv);
        Standard_Real G = dSdv.Dot(dSdv);
        Standard_Real L = d2Sdu2.Dot(normalVec);
        Standard_Real M = d2Sduv.Dot(normalVec);
        Standard_Real N = d2Sdv2.Dot(normalVec);
        Standard_Real denom = E * G - F * F;

        if (std::abs(denom) > 1e-20)
        {
          aResult.GaussianCurvature = (L * N - M * M) / denom;
          aResult.MeanCurvature = (E * N - 2.0 * F * M + G * L) / (2.0 * denom);
          Standard_Real disc = aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
          Standard_Real sqrtDisc = std::sqrt(std::max(0.0, disc));
          aResult.MinCurvature = aResult.MeanCurvature - sqrtDisc;
          aResult.MaxCurvature = aResult.MeanCurvature + sqrtDisc;
        }
      }
      else
      {
        aResult.Normal = gp_Dir(0, 0, 1);
      }

      myResults.push_back(aResult);
      myNbPnt = 1;
    }
  }

  myIsDone = Standard_True;
}

//=================================================================================================

void BRepIntCurveSurface_InterBVH::PerformBatch(
  const NCollection_Array1<gp_Lin>& theRays,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& theResults,
  const Standard_Integer theNumThreads)
{
  (void)theNumThreads;  // Will be used for manual thread control if needed

  if (!myIsLoaded)
  {
    theResults.Resize(theRays.Lower(), theRays.Upper(), Standard_False);
    return;
  }

  Standard_Integer nRays = theRays.Length();
  theResults.Resize(theRays.Lower(), theRays.Lower() + nRays - 1, Standard_False);

  // Initialize results
  for (Standard_Integer i = theResults.Lower(); i <= theResults.Upper(); ++i)
  {
    theResults(i) = BRepIntCurveSurface_HitResult();
  }

  auto startTime = std::chrono::high_resolution_clock::now();

  // Use tessellation-accelerated path
  if (myTriBVH.IsNull())
  {
    std::cerr << "Error: Triangle BVH not built - shape must be tessellated." << std::endl;
    return;
  }

  // Print backend info
  const char* backendNames[] = {"OCCT_BVH", "Embree_Scalar", "Embree_SIMD4", "Embree_SIMD8"};
  std::cout << "  Backend requested: " << backendNames[static_cast<int>(myBackend)]
            << ", OpenMP: " << (myUseOpenMP ? "enabled" : "disabled") << std::endl;
  std::cout << "  [DEBUG] myTriBVH Elements size: " << myTriBVH->Elements.size() << std::endl;
  std::cout << "  [DEBUG] myTriBVH Vertices size: " << myTriBVH->Vertices.size() << std::endl;
  std::cout << "  [DEBUG] myTriangleInfo size: " << myTriangleInfo.size() << std::endl;

  // Structure to hold thread-local stats (avoid atomic contention)
  struct ThreadLocalStats {
    long long faceTests = 0;
    long long newtonIters = 0;
    long long newtonFailures = 0;
    long long refinementTimeNs = 0;
    long long nodeTests = 0;  // BVH node tests (thread-local to avoid atomic contention)
  };

  // Thread-local surface adaptors type (for thread-safe surface evaluation)
  using ThreadLocalSurfaces = std::vector<Handle(Adaptor3d_Surface)>;

  // Lambda to process a single ray and refine the hit
  // Uses thread-local surface adaptors for thread safety
  auto processRayHit = [&](Standard_Integer idx,
                           Standard_Integer hitTriIdx,
                           Standard_Real hitT,
                           Standard_Real baryU,
                           Standard_Real baryV,
                           const gp_Lin& aRay,
                           ThreadLocalStats& stats,
                           ThreadLocalSurfaces& localSurfaces) {
    if (hitTriIdx < 0 || hitTriIdx >= static_cast<Standard_Integer>(myTriangleInfo.size()))
      return;

    Standard_Integer hitFaceIdx = myTriangleInfo[hitTriIdx].FaceIndex;
    if (hitFaceIdx < 0 || hitFaceIdx >= static_cast<Standard_Integer>(mySurfaceAdaptors.size()))
      return;

    stats.faceTests++;

    // UV-guided Newton refinement
    auto t0 = std::chrono::high_resolution_clock::now();

    const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[hitTriIdx];
    Standard_Real baryW = 1.0 - baryU - baryV;
    Standard_Real initU = baryW * triInfo.UV0.X() + baryU * triInfo.UV1.X() + baryV * triInfo.UV2.X();
    Standard_Real initV = baryW * triInfo.UV0.Y() + baryU * triInfo.UV1.Y() + baryV * triInfo.UV2.Y();

    // Use thread-local surface adaptor for thread safety
    const Adaptor3d_Surface& aSurface = *localSurfaces[hitFaceIdx];
    Standard_Real finalU = initU;
    Standard_Real finalV = initV;
    Standard_Real finalT = 0.0;
    gp_Pnt finalPnt;
    Standard_Integer iterCount = 0;

    NewtonResult newtonResult = RefineIntersectionNewton(
      aSurface, aRay.Location(), aRay.Direction(),
      finalU, finalV, finalT, finalPnt, iterCount,
      myTolerance, 100);

    auto t1 = std::chrono::high_resolution_clock::now();
    stats.refinementTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    stats.newtonIters += iterCount;

    if (hitT >= 0.0)
    {
      BRepIntCurveSurface_HitResult& aResult = theResults(idx);
      aResult.IsValid = Standard_True;

      if (newtonResult == NewtonResult::Converged && finalT >= 0.0)
      {
        aResult.Point = finalPnt;
        aResult.U = finalU;
        aResult.V = finalV;
        aResult.W = finalT;
      }
      else
      {
        stats.newtonFailures++;
        aResult.Point = aRay.Location().Translated(hitT * gp_Vec(aRay.Direction()));
        aResult.U = initU;
        aResult.V = initV;
        aResult.W = hitT;
      }

      aResult.FaceIndex = hitFaceIdx + 1;
      aResult.Transition = IntCurveSurface_In;
      aResult.State = TopAbs_IN;

      // Compute surface normal and curvatures
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv;
      aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
      gp_Vec normalVec = dSdu.Crossed(dSdv);
      Standard_Real normalMag = normalVec.Magnitude();

      if (normalMag > 1e-10)
      {
        normalVec.Normalize();
        const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
        if (aFace.Orientation() == TopAbs_REVERSED)
          normalVec.Reverse();
        aResult.Normal = gp_Dir(normalVec);

        Standard_Real E = dSdu.Dot(dSdu);
        Standard_Real F = dSdu.Dot(dSdv);
        Standard_Real G = dSdv.Dot(dSdv);
        Standard_Real L = d2Sdu2.Dot(normalVec);
        Standard_Real M = d2Sduv.Dot(normalVec);
        Standard_Real N = d2Sdv2.Dot(normalVec);

        Standard_Real denom = E * G - F * F;
        if (std::abs(denom) > 1e-20)
        {
          aResult.GaussianCurvature = (L * N - M * M) / denom;
          aResult.MeanCurvature = (E * N - 2.0 * F * M + G * L) / (2.0 * denom);
          Standard_Real disc = aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
          Standard_Real sqrtDisc = std::sqrt(std::max(0.0, disc));
          aResult.MinCurvature = aResult.MeanCurvature - sqrtDisc;
          aResult.MaxCurvature = aResult.MeanCurvature + sqrtDisc;
        }
      }
      else
      {
        aResult.Normal = gp_Dir(0, 0, 1);
      }
    }
  };

  // Helper to create thread-local surface copies
  auto createLocalSurfaces = [&]() -> ThreadLocalSurfaces {
    ThreadLocalSurfaces surfaces(mySurfaceAdaptors.size());
    for (size_t i = 0; i < mySurfaceAdaptors.size(); ++i) {
      surfaces[i] = mySurfaceAdaptors[i]->ShallowCopy();
    }
    return surfaces;
  };

  // Choose backend and parallelization strategy
  BRepIntCurveSurface_BVHBackend effectiveBackend = myBackend;

#ifdef OCCT_USE_EMBREE
  // Embree is available - check if we have a scene
  if (!myEmbreeScene && (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_Scalar ||
                          effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4 ||
                          effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8))
  {
    std::cout << "  Warning: Embree scene not built, falling back to OCCT_BVH" << std::endl;
    effectiveBackend = BRepIntCurveSurface_BVHBackend::OCCT_BVH;
  }
#else
  // Embree not available - force OCCT_BVH
  if (effectiveBackend != BRepIntCurveSurface_BVHBackend::OCCT_BVH)
  {
    std::cout << "  Warning: Embree not available, using OCCT_BVH" << std::endl;
    effectiveBackend = BRepIntCurveSurface_BVHBackend::OCCT_BVH;
  }
#endif

  // Aggregate stats from all threads
  ThreadLocalStats totalStats;

  // Process rays based on backend and parallelization settings
  if (effectiveBackend == BRepIntCurveSurface_BVHBackend::OCCT_BVH)
  {
    // OCCT BVH backend (no Embree dependency)
    #ifdef _OPENMP
    if (myUseOpenMP)
    {
      #pragma omp parallel
      {
        ThreadLocalStats localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces();  // Thread-local surface copies
        #pragma omp for schedule(dynamic, 64)
        for (Standard_Integer i = 0; i < nRays; ++i)
        {
          Standard_Integer idx = theRays.Lower() + i;
          const gp_Lin& aRay = theRays(idx);

          BRepIntCurveSurface_TriangleTraverser aTriTraverser;
          aTriTraverser.SetTriBVH(myTriBVH.get());
          aTriTraverser.SetTriangleInfo(&myTriangleInfo);
          aTriTraverser.SetRay(aRay, 0.0, RealLast());
          aTriTraverser.Select();

          // Accumulate thread-local node test count (no atomic overhead!)
          localStats.nodeTests += aTriTraverser.GetNodeTestCount();

          Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
          Standard_Real hitT = aTriTraverser.GetHitT();
          Standard_Real baryU, baryV;
          aTriTraverser.GetHitBarycentric(baryU, baryV);

          processRayHit(idx, hitTriIdx, hitT, baryU, baryV, aRay, localStats, localSurfaces);
        }
        #pragma omp critical
        {
          totalStats.faceTests += localStats.faceTests;
          totalStats.newtonIters += localStats.newtonIters;
          totalStats.newtonFailures += localStats.newtonFailures;
          totalStats.refinementTimeNs += localStats.refinementTimeNs;
          totalStats.nodeTests += localStats.nodeTests;
        }
      }
    }
    else
    #endif
    {
      ThreadLocalStats localStats;
      ThreadLocalSurfaces localSurfaces = createLocalSurfaces();  // Single-threaded uses same pattern
      for (Standard_Integer i = 0; i < nRays; ++i)
      {
        Standard_Integer idx = theRays.Lower() + i;
        const gp_Lin& aRay = theRays(idx);

        BRepIntCurveSurface_TriangleTraverser aTriTraverser;
        aTriTraverser.SetTriBVH(myTriBVH.get());
        aTriTraverser.SetTriangleInfo(&myTriangleInfo);
        aTriTraverser.SetRay(aRay, 0.0, RealLast());
        aTriTraverser.Select();

        // Accumulate thread-local node test count
        localStats.nodeTests += aTriTraverser.GetNodeTestCount();

        Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
        Standard_Real hitT = aTriTraverser.GetHitT();
        Standard_Real baryU, baryV;
        aTriTraverser.GetHitBarycentric(baryU, baryV);

        processRayHit(idx, hitTriIdx, hitT, baryU, baryV, aRay, localStats, localSurfaces);
      }
      totalStats = localStats;
    }
  }
#ifdef OCCT_USE_EMBREE
  else if (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_Scalar)
  {
    // Embree scalar backend (rtcIntersect1)
    #ifdef _OPENMP
    if (myUseOpenMP)
    {
      #pragma omp parallel
      {
        ThreadLocalStats localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
        #pragma omp for schedule(dynamic, 64)
        for (Standard_Integer i = 0; i < nRays; ++i)
        {
          Standard_Integer idx = theRays.Lower() + i;
          const gp_Lin& aRay = theRays(idx);

          Standard_Integer hitTriIdx;
          Standard_Real hitT, baryU, baryV;
          IntersectEmbree1(myEmbreeScene, aRay, hitTriIdx, hitT, baryU, baryV);

          processRayHit(idx, hitTriIdx, hitT, baryU, baryV, aRay, localStats, localSurfaces);
        }
        #pragma omp critical
        {
          totalStats.faceTests += localStats.faceTests;
          totalStats.newtonIters += localStats.newtonIters;
          totalStats.newtonFailures += localStats.newtonFailures;
          totalStats.refinementTimeNs += localStats.refinementTimeNs;
        }
      }
    }
    else
    #endif
    {
      ThreadLocalStats localStats;
      ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
      for (Standard_Integer i = 0; i < nRays; ++i)
      {
        Standard_Integer idx = theRays.Lower() + i;
        const gp_Lin& aRay = theRays(idx);

        Standard_Integer hitTriIdx;
        Standard_Real hitT, baryU, baryV;
        IntersectEmbree1(myEmbreeScene, aRay, hitTriIdx, hitT, baryU, baryV);

        processRayHit(idx, hitTriIdx, hitT, baryU, baryV, aRay, localStats, localSurfaces);
      }
      totalStats = localStats;
    }
  }
  else if (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4)
  {
    // Embree SIMD4 backend (rtcIntersect4) - process 4 rays at a time
    #ifdef _OPENMP
    if (myUseOpenMP)
    {
      #pragma omp parallel
      {
        ThreadLocalStats localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
        #pragma omp for schedule(dynamic, 16)
        for (Standard_Integer i = 0; i < nRays; i += 4)
        {
          Standard_Integer batchSize = std::min(4, nRays - i);

          // Prepare batch of rays (pad with dummy rays if needed)
          gp_Lin rays[4];
          for (int j = 0; j < 4; ++j)
          {
            if (j < batchSize)
              rays[j] = theRays(theRays.Lower() + i + j);
            else
              rays[j] = rays[0];  // Duplicate first ray for padding
          }

          Standard_Integer triIdx[4];
          Standard_Real hitT[4], baryU[4], baryV[4];
          IntersectEmbree4(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

          // Process results
          for (int j = 0; j < batchSize; ++j)
          {
            Standard_Integer idx = theRays.Lower() + i + j;
            processRayHit(idx, triIdx[j], hitT[j], baryU[j], baryV[j], rays[j], localStats, localSurfaces);
          }
        }
        #pragma omp critical
        {
          totalStats.faceTests += localStats.faceTests;
          totalStats.newtonIters += localStats.newtonIters;
          totalStats.newtonFailures += localStats.newtonFailures;
          totalStats.refinementTimeNs += localStats.refinementTimeNs;
        }
      }
    }
    else
    #endif
    {
      ThreadLocalStats localStats;
      ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
      for (Standard_Integer i = 0; i < nRays; i += 4)
      {
        Standard_Integer batchSize = std::min(4, nRays - i);

        gp_Lin rays[4];
        for (int j = 0; j < 4; ++j)
        {
          if (j < batchSize)
            rays[j] = theRays(theRays.Lower() + i + j);
          else
            rays[j] = rays[0];
        }

        Standard_Integer triIdx[4];
        Standard_Real hitT[4], baryU[4], baryV[4];
        IntersectEmbree4(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

        for (int j = 0; j < batchSize; ++j)
        {
          Standard_Integer idx = theRays.Lower() + i + j;
          processRayHit(idx, triIdx[j], hitT[j], baryU[j], baryV[j], rays[j], localStats, localSurfaces);
        }
      }
      totalStats = localStats;
    }
  }
  else if (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8)
  {
    // Embree SIMD8 backend (rtcIntersect8) - process 8 rays at a time
    #ifdef _OPENMP
    if (myUseOpenMP)
    {
      #pragma omp parallel
      {
        ThreadLocalStats localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
        #pragma omp for schedule(dynamic, 8)
        for (Standard_Integer i = 0; i < nRays; i += 8)
        {
          Standard_Integer batchSize = std::min(8, nRays - i);

          gp_Lin rays[8];
          for (int j = 0; j < 8; ++j)
          {
            if (j < batchSize)
              rays[j] = theRays(theRays.Lower() + i + j);
            else
              rays[j] = rays[0];
          }

          Standard_Integer triIdx[8];
          Standard_Real hitT[8], baryU[8], baryV[8];
          IntersectEmbree8(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

          for (int j = 0; j < batchSize; ++j)
          {
            Standard_Integer idx = theRays.Lower() + i + j;
            processRayHit(idx, triIdx[j], hitT[j], baryU[j], baryV[j], rays[j], localStats, localSurfaces);
          }
        }
        #pragma omp critical
        {
          totalStats.faceTests += localStats.faceTests;
          totalStats.newtonIters += localStats.newtonIters;
          totalStats.newtonFailures += localStats.newtonFailures;
          totalStats.refinementTimeNs += localStats.refinementTimeNs;
        }
      }
    }
    else
    #endif
    {
      ThreadLocalStats localStats;
      ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
      for (Standard_Integer i = 0; i < nRays; i += 8)
      {
        Standard_Integer batchSize = std::min(8, nRays - i);

        gp_Lin rays[8];
        for (int j = 0; j < 8; ++j)
        {
          if (j < batchSize)
            rays[j] = theRays(theRays.Lower() + i + j);
          else
            rays[j] = rays[0];
        }

        Standard_Integer triIdx[8];
        Standard_Real hitT[8], baryU[8], baryV[8];
        IntersectEmbree8(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

        for (int j = 0; j < batchSize; ++j)
        {
          Standard_Integer idx = theRays.Lower() + i + j;
          processRayHit(idx, triIdx[j], hitT[j], baryU[j], baryV[j], rays[j], localStats, localSurfaces);
        }
      }
      totalStats = localStats;
    }
  }
#endif

  // Print debug stats (using thread-local accumulated stats, no atomic contention)
  auto endTime = std::chrono::high_resolution_clock::now();
  double totalSec = std::chrono::duration<double>(endTime - startTime).count();

  std::cout << "  Total time: " << std::fixed << std::setprecision(2) << totalSec << "s, "
            << std::setprecision(0) << (nRays / totalSec) << " rays/sec" << std::endl;
  std::cout << "  [DEBUG] Triangle BVH node tests: " << totalStats.nodeTests
            << " (" << (double)totalStats.nodeTests / nRays << " per ray)" << std::endl;
  std::cout << "  [DEBUG] Face intersection tests: " << totalStats.faceTests
            << " (" << (double)totalStats.faceTests / nRays << " per ray)" << std::endl;
  if (totalStats.faceTests > 0)
  {
    std::cout << "  [DEBUG] Surface refinement time: "
              << totalStats.refinementTimeNs / 1000000.0 << " ms ("
              << (double)totalStats.refinementTimeNs / totalStats.faceTests / 1000.0 << " us per call)" << std::endl;
  }
  if (totalStats.newtonIters > 0)
  {
    std::cout << "  [DEBUG] Newton iterations: " << totalStats.newtonIters
              << " (" << (double)totalStats.newtonIters / totalStats.faceTests << " per hit)" << std::endl;
  }
  if (totalStats.newtonFailures > 0)
  {
    std::cout << "  [DEBUG] Newton refinement failures: " << totalStats.newtonFailures
              << " (used triangle fallback)" << std::endl;
  }
}

//=================================================================================================

void BRepIntCurveSurface_InterBVH::PerformBatchCount(
  const NCollection_Array1<gp_Lin>& theRays,
  NCollection_Array1<Standard_Integer>& theHitCounts,
  const Standard_Integer theNumThreads)
{
  if (!myIsLoaded)
  {
    theHitCounts.Resize(theRays.Lower(), theRays.Upper(), Standard_False);
    for (Standard_Integer i = theHitCounts.Lower(); i <= theHitCounts.Upper(); ++i)
      theHitCounts(i) = 0;
    return;
  }

  Standard_Integer nRays = theRays.Length();
  theHitCounts.Resize(theRays.Lower(), theRays.Lower() + nRays - 1, Standard_False);

  // Initialize counts
  for (Standard_Integer i = theHitCounts.Lower(); i <= theHitCounts.Upper(); ++i)
  {
    theHitCounts(i) = 0;
  }

  // Progress tracking
  Standard_Integer lastProgress = 0;
  auto startTime = std::chrono::high_resolution_clock::now();

  // Use tessellation-accelerated path
  if (myTriBVH.IsNull())
  {
    std::cerr << "Error: Triangle BVH not built - shape must be tessellated." << std::endl;
    return;
  }

  std::cout << "  Using tessellation-accelerated intersection counting..." << std::endl;

  // Thread-local node test accumulator
  long long totalNodeTests = 0;

  // Single-threaded triangle BVH traversal (can be parallelized later)
  for (Standard_Integer i = 0; i < nRays; ++i)
  {
    Standard_Integer idx = theRays.Lower() + i;
    const gp_Lin& aRay = theRays(idx);

    // Use the triangle count traverser (counts ALL triangle hits)
    BRepIntCurveSurface_TriangleCountTraverser aTriTraverser;
    aTriTraverser.SetTriBVH(myTriBVH.get());
    aTriTraverser.SetTriangleInfo(&myTriangleInfo);
    aTriTraverser.SetRay(aRay, 0.0, RealLast());
    aTriTraverser.Select();

    theHitCounts(idx) = aTriTraverser.GetHitCount();
    totalNodeTests += aTriTraverser.GetNodeTestCount();

    // Print progress every 5%
    Standard_Integer currentProgress = (i + 1) * 100 / nRays;
    if (currentProgress >= lastProgress + 5)
    {
      auto now = std::chrono::high_resolution_clock::now();
      double elapsed = std::chrono::duration<double>(now - startTime).count();
      std::cout << "  Progress: " << currentProgress << "% (" << std::fixed << std::setprecision(1) << elapsed << "s)" << std::endl;
      lastProgress = currentProgress;
    }
  }

  // Print debug stats
  std::cout << "  [DEBUG] Triangle BVH node tests: " << totalNodeTests
            << " (" << std::fixed << std::setprecision(1)
            << (double)totalNodeTests / nRays << " per ray)" << std::endl;
}

//=================================================================================================

const gp_Pnt& BRepIntCurveSurface_InterBVH::Pnt(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::Pnt - index out of range");
  return myResults[theIndex - 1].Point;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::U(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::U - index out of range");
  return myResults[theIndex - 1].U;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::V(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::V - index out of range");
  return myResults[theIndex - 1].V;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::W(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::W - index out of range");
  return myResults[theIndex - 1].W;
}

//=================================================================================================

const TopoDS_Face& BRepIntCurveSurface_InterBVH::Face(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::Face - index out of range");
  Standard_Integer aFaceIdx = myResults[theIndex - 1].FaceIndex;
  return TopoDS::Face(myFaces.FindKey(aFaceIdx));
}

//=================================================================================================

gp_Dir BRepIntCurveSurface_InterBVH::Normal(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::Normal - index out of range");
  return myResults[theIndex - 1].Normal;
}

//=================================================================================================

IntCurveSurface_TransitionOnCurve BRepIntCurveSurface_InterBVH::Transition(
  const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::Transition - index out of range");
  return myResults[theIndex - 1].Transition;
}

//=================================================================================================

TopAbs_State BRepIntCurveSurface_InterBVH::State(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::State - index out of range");
  return myResults[theIndex - 1].State;
}
