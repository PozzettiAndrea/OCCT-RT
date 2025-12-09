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
#include <BRepIntCurveSurface_RayPacket.hxx>
#include <BRepIntCurveSurface_SIMDSurface.hxx>

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
#include <gp_Pln.hxx>
#include <gp_Cone.hxx>
#include <gp_Torus.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Sphere.hxx>
#include <set>

// B-spline support for knot span caching and Bézier extraction
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <GeomConvert_BSplineSurfaceToBezierSurface.hxx>
#include <BSplCLib.hxx>
#include <BSplSLib.hxx>
#include <BSplSLib_Cache.hxx>

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
namespace
{
std::atomic<long long> g_nodeTestCount{0};
std::atomic<long long> g_faceTestCount{0};
std::atomic<long long> g_intersectorTimeNs{0};
std::atomic<long long> g_newtonIterations{0};
std::atomic<long long> g_newtonFailures{0};
// Newton failure breakdown counters
std::atomic<long long> g_newtonFailSingular{0}; // Singular Jacobian
std::atomic<long long> g_newtonFailMaxIter{0};  // Max iterations exceeded
std::atomic<long long> g_newtonFailResidual{0}; // Final residual too large

//! Newton refinement result
enum class NewtonResult
{
  Converged,
  SingularJacobian,
  MaxIterations,
  ResidualTooLarge
};

// Default vertex welding tolerance
constexpr double DEFAULT_WELD_TOLERANCE = 1.0e-3;

//! Spatial hash grid for vertex welding
//! Uses cell size = tolerance, checks 27 neighboring cells for proper tolerance matching
class SpatialVertexGrid
{
public:
  SpatialVertexGrid(double tolerance, size_t reserveSize)
      : myTol(tolerance),
        myTolSq(tolerance * tolerance)
  {
    myCells.reserve(reserveSize);
  }

  //! Find existing vertex within tolerance, or add new one
  //! Returns vertex index
  Standard_Integer FindOrAdd(const BVH_Vec3d& v, std::vector<BVH_Vec3d>& vertices)
  {
    int64_t cx = CellCoord(v[0]);
    int64_t cy = CellCoord(v[1]);
    int64_t cz = CellCoord(v[2]);

    // Check all 27 neighboring cells (including center)
    for (int dx = -1; dx <= 1; ++dx)
    {
      for (int dy = -1; dy <= 1; ++dy)
      {
        for (int dz = -1; dz <= 1; ++dz)
        {
          uint64_t cellKey = MakeCellKey(cx + dx, cy + dy, cz + dz);
          auto     it      = myCells.find(cellKey);
          if (it != myCells.end())
          {
            // Check all vertices in this cell
            for (Standard_Integer idx : it->second)
            {
              const BVH_Vec3d& existing = vertices[idx];
              double           dx2      = v[0] - existing[0];
              double           dy2      = v[1] - existing[1];
              double           dz2      = v[2] - existing[2];
              if (dx2 * dx2 + dy2 * dy2 + dz2 * dz2 < myTolSq)
              {
                return idx; // Found match
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
  int64_t CellCoord(double val) const { return static_cast<int64_t>(std::floor(val / myTol)); }

  uint64_t MakeCellKey(int64_t x, int64_t y, int64_t z) const
  {
    // Combine 3 coordinates into single key
    // Use 21 bits per coordinate (covers ±1 million cells)
    return ((static_cast<uint64_t>(x) & 0x1FFFFF)) | ((static_cast<uint64_t>(y) & 0x1FFFFF) << 21)
           | ((static_cast<uint64_t>(z) & 0x1FFFFF) << 42);
  }

  double                                                      myTol;
  double                                                      myTolSq;
  std::unordered_map<uint64_t, std::vector<Standard_Integer>> myCells;
};
} // namespace

//! Newton iteration to refine ray-surface intersection starting from approximate UV
//! Returns NewtonResult indicating convergence status
//! @param theSurface Surface adaptor
//! @param theRayOrigin Ray origin point
//! @param theRayDir Ray direction (normalized)
//! @param theU Initial U parameter (will be refined)
//! @param theV Initial V parameter (will be refined)
//! @param theT Output: parameter along ray
//! @param thePnt Output: intersection point
//! @param theIterCount Output: number of iterations performed (for stats, avoids atomics in hot
//! loop)
//! @param theTol Convergence tolerance
//! @param theMaxIter Maximum iterations
static NewtonResult RefineIntersectionNewton(const Adaptor3d_Surface& theSurface,
                                             const gp_Pnt&            theRayOrigin,
                                             const gp_Dir&            theRayDir,
                                             Standard_Real&           theU,
                                             Standard_Real&           theV,
                                             Standard_Real&           theT,
                                             gp_Pnt&                  thePnt,
                                             Standard_Integer&        theIterCount,
                                             const Standard_Real      theTol     = 1e-7,
                                             const Standard_Integer   theMaxIter = 10)
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
  Standard_Real    prevResidual = RealLast();  // For stall detection

  // Store best result for early exit
  Standard_Real bestU = u, bestV = v, bestT = 0.0;
  gp_Pnt        bestPnt;
  Standard_Real bestResidual = RealLast();

  Standard_Integer localIterCount = 0;
  for (Standard_Integer iter = 0; iter < theMaxIter; ++iter)
  {
    ++localIterCount;

    // Evaluate surface and derivatives
    theSurface.D1(u, v, S, dSu, dSv);

    // Compute t = (S - O) . D (projection of surface point onto ray)
    gp_Vec        SO(O, S);
    Standard_Real t = SO.Dot(D);

    // Residual vector: r = S - (O + t*D)
    gp_Pnt rayPt = O.Translated(t * D);
    gp_Vec r(rayPt, S);

    // Check convergence
    Standard_Real residual = r.Magnitude();

    // Track best result
    if (residual < bestResidual)
    {
      bestResidual = residual;
      bestU = u;
      bestV = v;
      bestT = t;
      bestPnt = S;
    }

    // OPTIMIZATION: Full convergence check
    if (residual < theTol)
    {
      theU         = u;
      theV         = v;
      theT         = t;
      thePnt       = S;
      theIterCount = localIterCount;
      return NewtonResult::Converged;
    }

    // OPTIMIZATION: Relaxed early exit after 2 iterations
    // Most hits converge quickly; accept "good enough" results
    if (iter >= 2 && residual < theTol * 100.0)
    {
      theU         = u;
      theV         = v;
      theT         = t;
      thePnt       = S;
      theIterCount = localIterCount;
      return NewtonResult::Converged;
    }

    // OPTIMIZATION: Stall detection - stop if not making progress
    // If residual hasn't decreased by at least 20%, we're likely stuck
    if (iter >= 1 && residual > prevResidual * 0.8)
    {
      // Not converging well, use best result so far
      theU         = bestU;
      theV         = bestV;
      theT         = bestT;
      thePnt       = bestPnt;
      theIterCount = localIterCount;
      // Accept if best result is reasonably good
      if (bestResidual < theTol * 10.0)
        return NewtonResult::Converged;
      break;
    }

    prevResidual = residual;

    // Build 2x2 Jacobian for Newton step in (u,v)
    // J = [ dSu.dSu  dSu.dSv ]
    //     [ dSv.dSu  dSv.dSv ]
    //
    // RHS = -[ r.dSu ]
    //       [ r.dSv ]

    Standard_Real j11 = dSu.Dot(dSu);
    Standard_Real j12 = dSu.Dot(dSv);
    Standard_Real j21 = j12; // symmetric
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
    Standard_Real stepSize = std::sqrt(du * du + dv * dv);
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

  // Did not converge within max iterations, return best result found
  theU   = bestU;
  theV   = bestV;
  theT   = bestT;
  thePnt = bestPnt;
  theIterCount = localIterCount;

  // Check if best result is acceptable
  if (bestResidual < theTol * 10.0)
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

//! Newton iteration using BSplSLib_Cache for fast B-spline evaluation
//! @param theBSpline B-spline surface handle
//! @param theCache BSplSLib_Cache (will be rebuilt if UV is outside cached span)
//! @param theRayOrigin Ray origin point
//! @param theRayDir Ray direction (normalized)
//! @param theU Initial U parameter (will be refined)
//! @param theV Initial V parameter (will be refined)
//! @param theT Output: parameter along ray
//! @param thePnt Output: intersection point
//! @param theIterCount Output: number of iterations performed
//! @param theTol Convergence tolerance
//! @param theMaxIter Maximum iterations
static NewtonResult RefineIntersectionNewtonBSpline(
  const Handle(Geom_BSplineSurface)& theBSpline,
  Handle(BSplSLib_Cache)&            theCache,
  const gp_Pnt&                      theRayOrigin,
  const gp_Dir&                      theRayDir,
  Standard_Real&                     theU,
  Standard_Real&                     theV,
  Standard_Real&                     theT,
  gp_Pnt&                            thePnt,
  Standard_Integer&                  theIterCount,
  const Standard_Real                theTol     = 1e-7,
  const Standard_Integer             theMaxIter = 10)
{
  Standard_Real u = theU;
  Standard_Real v = theV;

  // Get surface parameter bounds
  Standard_Real uMin = theBSpline->UKnot(1);
  Standard_Real uMax = theBSpline->UKnot(theBSpline->NbUKnots());
  Standard_Real vMin = theBSpline->VKnot(1);
  Standard_Real vMax = theBSpline->VKnot(theBSpline->NbVKnots());

  gp_Pnt S;
  gp_Vec dSu, dSv;
  gp_Vec D(theRayDir);
  gp_Pnt O = theRayOrigin;

  Standard_Boolean hitSingular  = Standard_False;
  Standard_Real    prevResidual = RealLast();

  // Store best result for early exit
  Standard_Real bestU = u, bestV = v, bestT = 0.0;
  gp_Pnt        bestPnt;
  Standard_Real bestResidual = RealLast();

  // Build cache if needed or if UV is outside current span
  if (theCache.IsNull() || !theCache->IsCacheValid(u, v))
  {
    // Create new cache or rebuild for this span
    if (theCache.IsNull())
    {
      // Get flat knots - size = NbPoles + Degree + 1
      TColStd_Array1OfReal uFlatKnots(1, theBSpline->NbUPoles() + theBSpline->UDegree() + 1);
      TColStd_Array1OfReal vFlatKnots(1, theBSpline->NbVPoles() + theBSpline->VDegree() + 1);

      // Build flat knot arrays from knots and multiplicities
      Standard_Integer idx = 1;
      for (Standard_Integer i = 1; i <= theBSpline->NbUKnots(); ++i)
      {
        for (Standard_Integer j = 0; j < theBSpline->UMultiplicity(i); ++j)
        {
          uFlatKnots(idx++) = theBSpline->UKnot(i);
        }
      }

      idx = 1;
      for (Standard_Integer i = 1; i <= theBSpline->NbVKnots(); ++i)
      {
        for (Standard_Integer j = 0; j < theBSpline->VMultiplicity(i); ++j)
        {
          vFlatKnots(idx++) = theBSpline->VKnot(i);
        }
      }

      // Check if surface is rational (has weights)
      const TColStd_Array2OfReal* weights = theBSpline->Weights();

      theCache = new BSplSLib_Cache(theBSpline->UDegree(), theBSpline->IsUPeriodic(), uFlatKnots,
                                    theBSpline->VDegree(), theBSpline->IsVPeriodic(), vFlatKnots,
                                    weights);
    }

    // Build cache for current span - flat knot size = NbPoles + Degree + 1
    TColStd_Array1OfReal uFlatKnots(1, theBSpline->NbUPoles() + theBSpline->UDegree() + 1);
    TColStd_Array1OfReal vFlatKnots(1, theBSpline->NbVPoles() + theBSpline->VDegree() + 1);

    Standard_Integer idx = 1;
    for (Standard_Integer i = 1; i <= theBSpline->NbUKnots(); ++i)
    {
      for (Standard_Integer j = 0; j < theBSpline->UMultiplicity(i); ++j)
      {
        uFlatKnots(idx++) = theBSpline->UKnot(i);
      }
    }

    idx = 1;
    for (Standard_Integer i = 1; i <= theBSpline->NbVKnots(); ++i)
    {
      for (Standard_Integer j = 0; j < theBSpline->VMultiplicity(i); ++j)
      {
        vFlatKnots(idx++) = theBSpline->VKnot(i);
      }
    }

    const TColStd_Array2OfReal* weights = theBSpline->Weights();

    theCache->BuildCache(u, v, uFlatKnots, vFlatKnots, theBSpline->Poles(), weights);
  }

  Standard_Integer localIterCount = 0;
  for (Standard_Integer iter = 0; iter < theMaxIter; ++iter)
  {
    ++localIterCount;

    // Use cache for fast D1 evaluation
    theCache->D1(u, v, S, dSu, dSv);

    // Compute t = (S - O) . D (projection of surface point onto ray)
    gp_Vec        SO(O, S);
    Standard_Real t = SO.Dot(D);

    // Residual vector: r = S - (O + t*D)
    gp_Pnt rayPt = O.Translated(t * D);
    gp_Vec r(rayPt, S);

    // Check convergence
    Standard_Real residual = r.Magnitude();

    // Track best result
    if (residual < bestResidual)
    {
      bestResidual = residual;
      bestU        = u;
      bestV        = v;
      bestT        = t;
      bestPnt      = S;
    }

    // Full convergence check
    if (residual < theTol)
    {
      theU         = u;
      theV         = v;
      theT         = t;
      thePnt       = S;
      theIterCount = localIterCount;
      return NewtonResult::Converged;
    }

    // Relaxed early exit after 2 iterations
    if (iter >= 2 && residual < theTol * 100.0)
    {
      theU         = u;
      theV         = v;
      theT         = t;
      thePnt       = S;
      theIterCount = localIterCount;
      return NewtonResult::Converged;
    }

    // Stall detection
    if (iter >= 1 && residual > prevResidual * 0.8)
    {
      theU         = bestU;
      theV         = bestV;
      theT         = bestT;
      thePnt       = bestPnt;
      theIterCount = localIterCount;
      if (bestResidual < theTol * 10.0)
        return NewtonResult::Converged;
      break;
    }

    prevResidual = residual;

    // Build 2x2 Jacobian for Newton step
    Standard_Real j11 = dSu.Dot(dSu);
    Standard_Real j12 = dSu.Dot(dSv);
    Standard_Real j21 = j12;
    Standard_Real j22 = dSv.Dot(dSv);

    Standard_Real b1 = -r.Dot(dSu);
    Standard_Real b2 = -r.Dot(dSv);

    // Solve 2x2 system
    Standard_Real det = j11 * j22 - j12 * j21;
    if (std::abs(det) < 1e-15)
    {
      hitSingular = Standard_True;
      break;
    }

    Standard_Real du = (j22 * b1 - j12 * b2) / det;
    Standard_Real dv = (j11 * b2 - j21 * b1) / det;

    // Damping
    Standard_Real stepSize = std::sqrt(du * du + dv * dv);
    if (stepSize > 0.5)
    {
      Standard_Real scale = 0.5 / stepSize;
      du *= scale;
      dv *= scale;
    }

    // Update parameters with bounds checking
    u += du;
    v += dv;

    u = std::max(uMin, std::min(uMax, u));
    v = std::max(vMin, std::min(vMax, v));

    // Check if UV moved to a different knot span - if so, rebuild cache
    if (!theCache->IsCacheValid(u, v))
    {
      TColStd_Array1OfReal uFlatKnots(1, theBSpline->NbUPoles() + theBSpline->UDegree() + 1);
      TColStd_Array1OfReal vFlatKnots(1, theBSpline->NbVPoles() + theBSpline->VDegree() + 1);

      Standard_Integer fidx = 1;
      for (Standard_Integer i = 1; i <= theBSpline->NbUKnots(); ++i)
      {
        for (Standard_Integer j = 0; j < theBSpline->UMultiplicity(i); ++j)
        {
          uFlatKnots(fidx++) = theBSpline->UKnot(i);
        }
      }

      fidx = 1;
      for (Standard_Integer i = 1; i <= theBSpline->NbVKnots(); ++i)
      {
        for (Standard_Integer j = 0; j < theBSpline->VMultiplicity(i); ++j)
        {
          vFlatKnots(fidx++) = theBSpline->VKnot(i);
        }
      }

      const TColStd_Array2OfReal* weights = theBSpline->Weights();

      theCache->BuildCache(u, v, uFlatKnots, vFlatKnots, theBSpline->Poles(), weights);
    }
  }

  // Return best result found
  theU         = bestU;
  theV         = bestV;
  theT         = bestT;
  thePnt       = bestPnt;
  theIterCount = localIterCount;

  if (bestResidual < theTol * 10.0)
  {
    return NewtonResult::Converged;
  }
  else if (hitSingular)
  {
    return NewtonResult::SingularJacobian;
  }
  else
  {
    return NewtonResult::MaxIterations;
  }
}

namespace
{
//! Möller–Trumbore ray-triangle intersection
//! Returns true if ray hits triangle, outputs t parameter and barycentric coords (u, v)
inline Standard_Boolean RayTriangleIntersect(const BVH_Vec3d& rayOrigin,
                                             const BVH_Vec3d& rayDir,
                                             const BVH_Vec3d& v0,
                                             const BVH_Vec3d& v1,
                                             const BVH_Vec3d& v2,
                                             Standard_Real&   t,
                                             Standard_Real&   u,
                                             Standard_Real&   v)
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
  BVH_Vec3d     s = rayOrigin - v0;

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
  {
  }

  void SetTriBVH(BRepIntCurveSurface_TriBVH* theBVH) { myTriBVH = theBVH; }

  void SetTriangleInfo(const std::vector<BRepIntCurveSurface_TriangleInfo>* theInfo)
  {
    myTriangleInfo = theInfo;
  }

  void SetRay(const gp_Lin& theRay, Standard_Real theMin, Standard_Real theMax)
  {
    myRayOrigin[0] = theRay.Location().X();
    myRayOrigin[1] = theRay.Location().Y();
    myRayOrigin[2] = theRay.Location().Z();
    myRayDir[0]    = theRay.Direction().X();
    myRayDir[1]    = theRay.Direction().Y();
    myRayDir[2]    = theRay.Direction().Z();

    // Precompute inverse direction for fast ray-box intersection
    // Using a small epsilon to avoid division by zero
    const Standard_Real epsilon = 1e-12;
    for (int i = 0; i < 3; ++i)
    {
      myInvRayDir[i] = (std::abs(myRayDir[i]) > epsilon)
                         ? (1.0 / myRayDir[i])
                         : (myRayDir[i] >= 0 ? 1.0 / epsilon : -1.0 / epsilon);
    }

    myMinParam         = theMin;
    myMaxParam         = theMax;
    myClosestT         = theMax;
    myHitTriangleIndex = -1;
    myHitFaceIndex     = -1;
    myHitBaryU         = 0.0;
    myHitBaryV         = 0.0;
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

      ++myNodeTestCount; // Thread-local counter (no atomic overhead)

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
        Standard_Integer aLastTriIdx  = aBVH->EndPrimitive(aNodeIdx);

        for (Standard_Integer triIdx = aFirstTriIdx; triIdx <= aLastTriIdx; ++triIdx)
        {
          TestTriangle(triIdx);
        }
      }
      else
      {
        // Inner node - push children (closer child first for efficiency)
        Standard_Integer aLeftChild  = aBVH->Child<0>(aNodeIdx);
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
  Standard_Boolean RayBoxIntersect(const BVH_Vec3d& boxMin,
                                   const BVH_Vec3d& boxMax,
                                   Standard_Real&   tNear,
                                   Standard_Real&   tFar) const
  {
    // Optimized slab method using precomputed inverse direction
    Standard_Real t1 = (boxMin[0] - myRayOrigin[0]) * myInvRayDir[0];
    Standard_Real t2 = (boxMax[0] - myRayOrigin[0]) * myInvRayDir[0];

    tNear = std::min(t1, t2);
    tFar  = std::max(t1, t2);

    t1 = (boxMin[1] - myRayOrigin[1]) * myInvRayDir[1];
    t2 = (boxMax[1] - myRayOrigin[1]) * myInvRayDir[1];

    tNear = std::max(tNear, std::min(t1, t2));
    tFar  = std::min(tFar, std::max(t1, t2));

    t1 = (boxMin[2] - myRayOrigin[2]) * myInvRayDir[2];
    t2 = (boxMax[2] - myRayOrigin[2]) * myInvRayDir[2];

    tNear = std::max(tNear, std::min(t1, t2));
    tFar  = std::min(tFar, std::max(t1, t2));

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
    if (elem[0] < 0 || elem[0] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size())
        || elem[1] < 0 || elem[1] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size())
        || elem[2] < 0 || elem[2] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()))
      return;

    // Get original triangle index from 4th component
    Standard_Integer originalTriIdx = elem[3];
    if (originalTriIdx < 0
        || originalTriIdx >= static_cast<Standard_Integer>(myTriangleInfo->size()))
      return;

    const BVH_Vec3d& v0 = myTriBVH->Vertices[elem[0]];
    const BVH_Vec3d& v1 = myTriBVH->Vertices[elem[1]];
    const BVH_Vec3d& v2 = myTriBVH->Vertices[elem[2]];

    Standard_Real t, u, v;
    if (RayTriangleIntersect(myRayOrigin, myRayDir, v0, v1, v2, t, u, v))
    {
      if (t >= myMinParam && t < myClosestT)
      {
        myClosestT         = t;
        myHitTriangleIndex = originalTriIdx; // Use ORIGINAL index for myTriangleInfo lookup
        myHitFaceIndex     = (*myTriangleInfo)[originalTriIdx].FaceIndex;
        myHitBaryU         = u;
        myHitBaryV         = v;
      }
    }
  }

  BRepIntCurveSurface_TriBVH*                          myTriBVH;
  const std::vector<BRepIntCurveSurface_TriangleInfo>* myTriangleInfo;
  BVH_Vec3d                                            myRayOrigin;
  BVH_Vec3d                                            myRayDir;
  BVH_Vec3d        myInvRayDir; // Precomputed 1/direction for fast ray-box tests
  Standard_Real    myClosestT;
  Standard_Integer myHitTriangleIndex;
  Standard_Integer myHitFaceIndex;
  Standard_Real    myMinParam;
  Standard_Real    myMaxParam;
  Standard_Real    myHitBaryU;
  Standard_Real    myHitBaryV;
  Standard_Integer myNodeTestCount; // Thread-local counter (no atomic overhead)
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
  {
  }

  void SetTriBVH(BRepIntCurveSurface_TriBVH* theBVH) { myTriBVH = theBVH; }

  void SetTriangleInfo(const std::vector<BRepIntCurveSurface_TriangleInfo>* theInfo)
  {
    myTriangleInfo = theInfo;
  }

  void SetRay(const gp_Lin& theRay, Standard_Real theMin, Standard_Real theMax)
  {
    myRayOrigin[0] = theRay.Location().X();
    myRayOrigin[1] = theRay.Location().Y();
    myRayOrigin[2] = theRay.Location().Z();
    myRayDir[0]    = theRay.Direction().X();
    myRayDir[1]    = theRay.Direction().Y();
    myRayDir[2]    = theRay.Direction().Z();

    // Precompute inverse direction for fast ray-box intersection
    const Standard_Real epsilon = 1e-12;
    for (int i = 0; i < 3; ++i)
    {
      myInvRayDir[i] = (std::abs(myRayDir[i]) > epsilon)
                         ? (1.0 / myRayDir[i])
                         : (myRayDir[i] >= 0 ? 1.0 / epsilon : -1.0 / epsilon);
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

      ++myNodeTestCount; // Thread-local counter (no atomic overhead)

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
        Standard_Integer aLastTriIdx  = aBVH->EndPrimitive(aNodeIdx);

        for (Standard_Integer triIdx = aFirstTriIdx; triIdx <= aLastTriIdx; ++triIdx)
        {
          TestTriangle(triIdx);
        }
      }
      else
      {
        // Inner node - push children
        Standard_Integer aLeftChild  = aBVH->Child<0>(aNodeIdx);
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
  Standard_Boolean RayBoxIntersect(const BVH_Vec3d& boxMin,
                                   const BVH_Vec3d& boxMax,
                                   Standard_Real&   tNear,
                                   Standard_Real&   tFar) const
  {
    Standard_Real t1 = (boxMin[0] - myRayOrigin[0]) * myInvRayDir[0];
    Standard_Real t2 = (boxMax[0] - myRayOrigin[0]) * myInvRayDir[0];

    tNear = std::min(t1, t2);
    tFar  = std::max(t1, t2);

    t1 = (boxMin[1] - myRayOrigin[1]) * myInvRayDir[1];
    t2 = (boxMax[1] - myRayOrigin[1]) * myInvRayDir[1];

    tNear = std::max(tNear, std::min(t1, t2));
    tFar  = std::min(tFar, std::max(t1, t2));

    t1 = (boxMin[2] - myRayOrigin[2]) * myInvRayDir[2];
    t2 = (boxMax[2] - myRayOrigin[2]) * myInvRayDir[2];

    tNear = std::max(tNear, std::min(t1, t2));
    tFar  = std::min(tFar, std::max(t1, t2));

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

    if (elem[0] < 0 || elem[0] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size())
        || elem[1] < 0 || elem[1] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size())
        || elem[2] < 0 || elem[2] >= static_cast<Standard_Integer>(myTriBVH->Vertices.size()))
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

  BRepIntCurveSurface_TriBVH*                          myTriBVH;
  const std::vector<BRepIntCurveSurface_TriangleInfo>* myTriangleInfo;
  BVH_Vec3d                                            myRayOrigin;
  BVH_Vec3d                                            myRayDir;
  BVH_Vec3d                                            myInvRayDir;
  Standard_Integer                                     myHitCount;
  Standard_Real                                        myMinParam;
  Standard_Real                                        myMaxParam;
  Standard_Integer myNodeTestCount; // Thread-local counter (no atomic overhead)
};
} // namespace

//=================================================================================================

BRepIntCurveSurface_InterBVH::BRepIntCurveSurface_InterBVH()
    : myTolerance(Precision::Confusion()),
      myDeflection(0.0),
      myUseTessellation(Standard_False),
      myIsLoaded(Standard_False),
      myIsDone(Standard_False),
      myNbPnt(0),
      myBackend(BRepIntCurveSurface_BVHBackend::OCCT_BVH), // Default to fastest single-ray
      myUseOpenMP(Standard_True),                          // Enable OpenMP by default
      myPacketSize(8),                                     // Default to 8-ray packets
      myEnableRaySorting(Standard_True),                   // Enable ray sorting by default
      myComputeCurvature(Standard_True),                   // Compute curvatures by default
      mySkipNewtonForPlanar(Standard_True),                // Skip Newton for planar faces by default
      myNewtonTolerance(1e-7),                             // Default Newton tolerance
      myNewtonMaxIter(10),                                 // Default max Newton iterations
      myUseSIMDNewton(Standard_False),                     // SIMD Newton disabled by default (experimental)
      myNumBSplineCacheThreads(1)                          // B-spline cache thread count
#ifdef OCCT_USE_EMBREE
      ,
      myEmbreeDevice(nullptr),
      myEmbreeScene(nullptr)
#endif
{
}

//=================================================================================================

BRepIntCurveSurface_InterBVH::~BRepIntCurveSurface_InterBVH()
{
#ifdef OCCT_USE_EMBREE
  if (myEmbreeScene)
  {
    rtcReleaseScene(myEmbreeScene);
    myEmbreeScene = nullptr;
  }
  if (myEmbreeDevice)
  {
    rtcReleaseDevice(myEmbreeDevice);
    myEmbreeDevice = nullptr;
  }
#endif
}

//=================================================================================================
// SIMD helpers for Embree batch intersection
//=================================================================================================

#ifdef OCCT_USE_EMBREE
namespace
{
//! Process 4 rays at once using SSE (rtcIntersect4)
//! @param theScene Embree scene
//! @param theRays Array of 4 rays (input)
//! @param theTriIdx Output: triangle indices (-1 if miss)
//! @param theHitT Output: hit distances
//! @param theBaryU Output: barycentric U coordinates
//! @param theBaryV Output: barycentric V coordinates
void IntersectEmbree4(RTCScene          theScene,
                      const gp_Lin*     theRays,
                      Standard_Integer* theTriIdx,
                      Standard_Real*    theHitT,
                      Standard_Real*    theBaryU,
                      Standard_Real*    theBaryV)
{
  alignas(16) RTCRayHit4 rayhit4;
  alignas(16) int        valid[4] = {-1, -1, -1, -1};

  // Setup 4 rays
  for (int i = 0; i < 4; ++i)
  {
    rayhit4.ray.org_x[i]  = static_cast<float>(theRays[i].Location().X());
    rayhit4.ray.org_y[i]  = static_cast<float>(theRays[i].Location().Y());
    rayhit4.ray.org_z[i]  = static_cast<float>(theRays[i].Location().Z());
    rayhit4.ray.dir_x[i]  = static_cast<float>(theRays[i].Direction().X());
    rayhit4.ray.dir_y[i]  = static_cast<float>(theRays[i].Direction().Y());
    rayhit4.ray.dir_z[i]  = static_cast<float>(theRays[i].Direction().Z());
    rayhit4.ray.tnear[i]  = 0.0f;
    rayhit4.ray.tfar[i]   = std::numeric_limits<float>::infinity();
    rayhit4.ray.mask[i]   = static_cast<unsigned int>(-1);
    rayhit4.ray.flags[i]  = 0;
    rayhit4.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
    rayhit4.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
  }

  // Embree 4 changed the API - args is now optional (pass NULL)
  rtcIntersect4(valid, theScene, &rayhit4, NULL);

  // Extract results
  for (int i = 0; i < 4; ++i)
  {
    if (rayhit4.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID)
    {
      theTriIdx[i] = static_cast<Standard_Integer>(rayhit4.hit.primID[i]);
      theHitT[i]   = static_cast<Standard_Real>(rayhit4.ray.tfar[i]);
      theBaryU[i]  = static_cast<Standard_Real>(rayhit4.hit.u[i]);
      theBaryV[i]  = static_cast<Standard_Real>(rayhit4.hit.v[i]);
    }
    else
    {
      theTriIdx[i] = -1;
      theHitT[i]   = -1.0;
      theBaryU[i]  = 0.0;
      theBaryV[i]  = 0.0;
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
void IntersectEmbree8(RTCScene          theScene,
                      const gp_Lin*     theRays,
                      Standard_Integer* theTriIdx,
                      Standard_Real*    theHitT,
                      Standard_Real*    theBaryU,
                      Standard_Real*    theBaryV)
{
  alignas(32) RTCRayHit8 rayhit8;
  alignas(32) int        valid[8] = {-1, -1, -1, -1, -1, -1, -1, -1};

  // Setup 8 rays
  for (int i = 0; i < 8; ++i)
  {
    rayhit8.ray.org_x[i]  = static_cast<float>(theRays[i].Location().X());
    rayhit8.ray.org_y[i]  = static_cast<float>(theRays[i].Location().Y());
    rayhit8.ray.org_z[i]  = static_cast<float>(theRays[i].Location().Z());
    rayhit8.ray.dir_x[i]  = static_cast<float>(theRays[i].Direction().X());
    rayhit8.ray.dir_y[i]  = static_cast<float>(theRays[i].Direction().Y());
    rayhit8.ray.dir_z[i]  = static_cast<float>(theRays[i].Direction().Z());
    rayhit8.ray.tnear[i]  = 0.0f;
    rayhit8.ray.tfar[i]   = std::numeric_limits<float>::infinity();
    rayhit8.ray.mask[i]   = static_cast<unsigned int>(-1);
    rayhit8.ray.flags[i]  = 0;
    rayhit8.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
    rayhit8.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
  }

  // Embree 4 changed the API - args is now optional (pass NULL)
  rtcIntersect8(valid, theScene, &rayhit8, NULL);

  // Extract results
  for (int i = 0; i < 8; ++i)
  {
    if (rayhit8.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID)
    {
      theTriIdx[i] = static_cast<Standard_Integer>(rayhit8.hit.primID[i]);
      theHitT[i]   = static_cast<Standard_Real>(rayhit8.ray.tfar[i]);
      theBaryU[i]  = static_cast<Standard_Real>(rayhit8.hit.u[i]);
      theBaryV[i]  = static_cast<Standard_Real>(rayhit8.hit.v[i]);
    }
    else
    {
      theTriIdx[i] = -1;
      theHitT[i]   = -1.0;
      theBaryU[i]  = 0.0;
      theBaryV[i]  = 0.0;
    }
  }
}

//! Single ray intersection using Embree (rtcIntersect1)
void IntersectEmbree1(RTCScene          theScene,
                      const gp_Lin&     theRay,
                      Standard_Integer& theTriIdx,
                      Standard_Real&    theHitT,
                      Standard_Real&    theBaryU,
                      Standard_Real&    theBaryV)
{
  RTCRayHit rayhit;
  rayhit.ray.org_x  = static_cast<float>(theRay.Location().X());
  rayhit.ray.org_y  = static_cast<float>(theRay.Location().Y());
  rayhit.ray.org_z  = static_cast<float>(theRay.Location().Z());
  rayhit.ray.dir_x  = static_cast<float>(theRay.Direction().X());
  rayhit.ray.dir_y  = static_cast<float>(theRay.Direction().Y());
  rayhit.ray.dir_z  = static_cast<float>(theRay.Direction().Z());
  rayhit.ray.tnear  = 0.0f;
  rayhit.ray.tfar   = std::numeric_limits<float>::infinity();
  rayhit.ray.mask   = static_cast<unsigned int>(-1);
  rayhit.ray.flags  = 0;
  rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayhit.hit.primID = RTC_INVALID_GEOMETRY_ID;

  // Embree 4 changed the API - args is now optional (pass NULL)
  rtcIntersect1(theScene, &rayhit, NULL);

  if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
  {
    theTriIdx = static_cast<Standard_Integer>(rayhit.hit.primID);
    theHitT   = static_cast<Standard_Real>(rayhit.ray.tfar);
    theBaryU  = static_cast<Standard_Real>(rayhit.hit.u);
    theBaryV  = static_cast<Standard_Real>(rayhit.hit.v);
  }
  else
  {
    theTriIdx = -1;
    theHitT   = -1.0;
    theBaryU  = 0.0;
    theBaryV  = 0.0;
  }
}
//! Process 8 rays from a packet using AVX (rtcIntersect8) with pre-sorted coherent rays
//! This version takes a RayPacket8 directly for optimal cache access
void IntersectEmbreePacket8(RTCScene                                            theScene,
                            const BRepIntCurveSurface_RayPacket<PACKET_SIZE_8>& thePacket,
                            BRepIntCurveSurface_PacketHitResult<PACKET_SIZE_8>& theResult)
{
  alignas(32) RTCRayHit8 rayhit8;
  alignas(32) int        valid[8];

  // Setup 8 rays from packet (already in SoA format - optimal!)
  for (int i = 0; i < 8; ++i)
  {
    // Only enable rays that are active in the packet
    valid[i] = (thePacket.ActiveMask & (1u << i)) ? -1 : 0;

    rayhit8.ray.org_x[i]  = static_cast<float>(thePacket.OrgX[i]);
    rayhit8.ray.org_y[i]  = static_cast<float>(thePacket.OrgY[i]);
    rayhit8.ray.org_z[i]  = static_cast<float>(thePacket.OrgZ[i]);
    rayhit8.ray.dir_x[i]  = static_cast<float>(thePacket.DirX[i]);
    rayhit8.ray.dir_y[i]  = static_cast<float>(thePacket.DirY[i]);
    rayhit8.ray.dir_z[i]  = static_cast<float>(thePacket.DirZ[i]);
    rayhit8.ray.tnear[i]  = static_cast<float>(thePacket.TMin[i]);
    rayhit8.ray.tfar[i]   = std::numeric_limits<float>::infinity();
    rayhit8.ray.mask[i]   = static_cast<unsigned int>(-1);
    rayhit8.ray.flags[i]  = 0;
    rayhit8.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
    rayhit8.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
  }

  // Embree 4 changed the API - args is now optional (pass NULL)
  rtcIntersect8(valid, theScene, &rayhit8, NULL);

  // Extract results back to packet result
  theResult.HitMask = 0;
  for (int i = 0; i < 8; ++i)
  {
    if (rayhit8.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID)
    {
      theResult.TriIdx[i]  = static_cast<Standard_Integer>(rayhit8.hit.primID[i]);
      theResult.HitT[i]    = static_cast<Standard_Real>(rayhit8.ray.tfar[i]);
      theResult.BaryU[i]   = static_cast<Standard_Real>(rayhit8.hit.u[i]);
      theResult.BaryV[i]   = static_cast<Standard_Real>(rayhit8.hit.v[i]);
      theResult.HitMask   |= (1u << i);
    }
    else
    {
      theResult.TriIdx[i] = -1;
      theResult.HitT[i]   = 1e30;
      theResult.BaryU[i]  = 0.0;
      theResult.BaryV[i]  = 0.0;
    }
  }
}

//! Process 4 rays from a packet using SSE (rtcIntersect4)
void IntersectEmbreePacket4(RTCScene                                            theScene,
                            const BRepIntCurveSurface_RayPacket<PACKET_SIZE_4>& thePacket,
                            BRepIntCurveSurface_PacketHitResult<PACKET_SIZE_4>& theResult)
{
  alignas(16) RTCRayHit4 rayhit4;
  alignas(16) int        valid[4];

  // Setup 4 rays from packet
  for (int i = 0; i < 4; ++i)
  {
    valid[i] = (thePacket.ActiveMask & (1u << i)) ? -1 : 0;

    rayhit4.ray.org_x[i]  = static_cast<float>(thePacket.OrgX[i]);
    rayhit4.ray.org_y[i]  = static_cast<float>(thePacket.OrgY[i]);
    rayhit4.ray.org_z[i]  = static_cast<float>(thePacket.OrgZ[i]);
    rayhit4.ray.dir_x[i]  = static_cast<float>(thePacket.DirX[i]);
    rayhit4.ray.dir_y[i]  = static_cast<float>(thePacket.DirY[i]);
    rayhit4.ray.dir_z[i]  = static_cast<float>(thePacket.DirZ[i]);
    rayhit4.ray.tnear[i]  = static_cast<float>(thePacket.TMin[i]);
    rayhit4.ray.tfar[i]   = std::numeric_limits<float>::infinity();
    rayhit4.ray.mask[i]   = static_cast<unsigned int>(-1);
    rayhit4.ray.flags[i]  = 0;
    rayhit4.hit.geomID[i] = RTC_INVALID_GEOMETRY_ID;
    rayhit4.hit.primID[i] = RTC_INVALID_GEOMETRY_ID;
  }

  rtcIntersect4(valid, theScene, &rayhit4, NULL);

  // Extract results
  theResult.HitMask = 0;
  for (int i = 0; i < 4; ++i)
  {
    if (rayhit4.hit.geomID[i] != RTC_INVALID_GEOMETRY_ID)
    {
      theResult.TriIdx[i]  = static_cast<Standard_Integer>(rayhit4.hit.primID[i]);
      theResult.HitT[i]    = static_cast<Standard_Real>(rayhit4.ray.tfar[i]);
      theResult.BaryU[i]   = static_cast<Standard_Real>(rayhit4.hit.u[i]);
      theResult.BaryV[i]   = static_cast<Standard_Real>(rayhit4.hit.v[i]);
      theResult.HitMask   |= (1u << i);
    }
    else
    {
      theResult.TriIdx[i] = -1;
      theResult.HitT[i]   = 1e30;
      theResult.BaryU[i]  = 0.0;
      theResult.BaryV[i]  = 0.0;
    }
  }
}

} // namespace
#endif

//=================================================================================================

void BRepIntCurveSurface_InterBVH::Load(const TopoDS_Shape& theShape,
                                        const Standard_Real theTol,
                                        const Standard_Real theDeflection)
{
  myIsLoaded = Standard_False;
  myIsDone   = Standard_False;
  myNbPnt    = 0;
  myResults.clear();
  myFaces.Clear();
  mySurfaceAdaptors.clear();
  myIsPlanarFace.clear();
  myBSplineSurfaces.clear();
  myBSplineCaches.clear();
  myNumBSplineCacheThreads = 1;
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

  // Create surface adaptors for all faces, classify by surface type, and cache orientations
  mySurfaceAdaptors.resize(myFaces.Extent());
  myIsPlanarFace.resize(myFaces.Extent());
  myFaceReversed.resize(myFaces.Extent());
  myBSplineSurfaces.resize(myFaces.Extent());
  Standard_Integer nPlanar = 0;
  Standard_Integer nBSpline = 0;

  for (Standard_Integer i = 1; i <= myFaces.Extent(); ++i)
  {
    const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(i));
    mySurfaceAdaptors[i - 1] = new BRepAdaptor_Surface(aFace, Standard_True);

    // Classify face by surface type for Newton optimization
    GeomAbs_SurfaceType surfType = mySurfaceAdaptors[i - 1]->GetType();
    myIsPlanarFace[i - 1] = (surfType == GeomAbs_Plane);
    if (myIsPlanarFace[i - 1])
      ++nPlanar;

    // Extract B-spline surface handle for cached evaluation
    if (surfType == GeomAbs_BSplineSurface)
    {
      myBSplineSurfaces[i - 1] = mySurfaceAdaptors[i - 1]->BSpline();
      ++nBSpline;
    }
    else
    {
      myBSplineSurfaces[i - 1].Nullify();
    }

    // Cache face orientation to avoid FindKey lookup in hot path
    myFaceReversed[i - 1] = (aFace.Orientation() == TopAbs_REVERSED);
  }

  std::cout << "  Face classification: " << nPlanar << "/" << myFaces.Extent()
            << " planar faces (Newton will be skipped for these)" << std::endl;
  if (nBSpline > 0)
  {
    std::cout << "  B-spline surfaces: " << nBSpline << " faces (using BSplSLib_Cache for fast eval)"
              << std::endl;
  }

  // Allocate B-spline caches for multi-threaded evaluation
  // One cache per B-spline face per thread to avoid contention
#ifdef _OPENMP
  myNumBSplineCacheThreads = omp_get_max_threads();
#else
  myNumBSplineCacheThreads = 1;
#endif
  myBSplineCaches.resize(myFaces.Extent() * myNumBSplineCacheThreads);

  // Build triangle BVH for tessellation-accelerated intersection
  // NOTE: The shape must already be tessellated before calling Load().
  // Call BRepMesh_IncrementalMesh on the shape before Load() if needed.
  {
    std::cout << "  Building tessellation BVH (using existing triangulation)..." << std::endl;

    // Count total triangles first
    Standard_Integer totalTriangles = 0;
    for (Standard_Integer faceIdx = 1; faceIdx <= myFaces.Extent(); ++faceIdx)
    {
      const TopoDS_Face&         aFace = TopoDS::Face(myFaces.FindKey(faceIdx));
      TopLoc_Location            aLoc;
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
        const TopoDS_Face&         aFace = TopoDS::Face(myFaces.FindKey(faceIdx));
        TopLoc_Location            aLoc;
        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(aFace, aLoc);

        if (aTriangulation.IsNull())
          continue;

        const gp_Trsf&   aTrsf        = aLoc.Transformation();
        Standard_Boolean hasTransform = (aLoc.IsIdentity() == Standard_False);

        // Get the UV nodes if available
        Standard_Boolean hasUVNodes = aTriangulation->HasUVNodes();

        for (Standard_Integer triIdx = 1; triIdx <= aTriangulation->NbTriangles(); ++triIdx)
        {
          const Poly_Triangle& aTri = aTriangulation->Triangle(triIdx);
          Standard_Integer     n1, n2, n3;
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
          aTriInfo.FaceIndex = faceIdx - 1; // 0-based

          if (hasUVNodes)
          {
            aTriInfo.UV0 = aTriangulation->UVNode(n1);
            aTriInfo.UV1 = aTriangulation->UVNode(n2);
            aTriInfo.UV2 = aTriangulation->UVNode(n3);

            // For B-spline surfaces: compute knot span indices for triangle centroid
            // This enables fast local Bézier evaluation during Newton refinement
            GeomAbs_SurfaceType surfType = mySurfaceAdaptors[faceIdx - 1]->GetType();
            if (surfType == GeomAbs_BSplineSurface)
            {
              // Compute centroid UV
              Standard_Real centU = (aTriInfo.UV0.X() + aTriInfo.UV1.X() + aTriInfo.UV2.X()) / 3.0;
              Standard_Real centV = (aTriInfo.UV0.Y() + aTriInfo.UV1.Y() + aTriInfo.UV2.Y()) / 3.0;

              // Get the B-spline surface
              Handle(Geom_BSplineSurface) bspline = mySurfaceAdaptors[faceIdx - 1]->BSpline();
              if (!bspline.IsNull())
              {
                // Get knot arrays
                const TColStd_Array1OfReal& uKnots = bspline->UKnots();
                const TColStd_Array1OfReal& vKnots = bspline->VKnots();

                // Find U knot span (binary search)
                aTriInfo.USpanIndex = 0;
                for (Standard_Integer k = uKnots.Lower(); k < uKnots.Upper(); ++k)
                {
                  if (centU >= uKnots(k) && centU < uKnots(k + 1))
                  {
                    aTriInfo.USpanIndex = k - uKnots.Lower();
                    break;
                  }
                }
                // Handle edge case: centU == last knot
                if (centU >= uKnots(uKnots.Upper()))
                  aTriInfo.USpanIndex = uKnots.Upper() - uKnots.Lower() - 1;

                // Find V knot span
                aTriInfo.VSpanIndex = 0;
                for (Standard_Integer k = vKnots.Lower(); k < vKnots.Upper(); ++k)
                {
                  if (centV >= vKnots(k) && centV < vKnots(k + 1))
                  {
                    aTriInfo.VSpanIndex = k - vKnots.Lower();
                    break;
                  }
                }
                if (centV >= vKnots(vKnots.Upper()))
                  aTriInfo.VSpanIndex = vKnots.Upper() - vKnots.Lower() - 1;
              }
            }
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
      Standard_Integer nVertices    = static_cast<Standard_Integer>(uniqueVertices.size());
      Standard_Integer nTriangles   = static_cast<Standard_Integer>(myTriangleInfo.size());
      Standard_Integer nRawVertices = totalTriangles * 3;

      // Report vertex welding stats
      std::cout << "  Vertex welding: " << nRawVertices << " -> " << nVertices << " (" << std::fixed
                << std::setprecision(1) << (100.0 * nVertices / nRawVertices) << "% unique)"
                << std::endl;

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
        myTriBVH->Elements[i] = BVH_Vec4i(triangleIndices[i * 3 + 0],
                                          triangleIndices[i * 3 + 1],
                                          triangleIndices[i * 3 + 2],
                                          i); // w = original triangle index
      }

      // CRITICAL: Mark as dirty so BVH() will actually build the tree
      myTriBVH->MarkDirty();

      // Build the BVH
      myTriBVH->BVH();

      myUseTessellation = Standard_True;

      std::cout << "  Triangle BVH built: " << nTriangles << " triangles from " << myFaces.Extent()
                << " faces" << std::endl;

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
      if (!myEmbreeDevice)
      {
        myEmbreeDevice = rtcNewDevice(nullptr);
        if (!myEmbreeDevice)
        {
          std::cerr << "  ERROR: Failed to create Embree device!" << std::endl;
        }
      }

      // Release old scene if exists
      if (myEmbreeScene)
      {
        rtcReleaseScene(myEmbreeScene);
        myEmbreeScene = nullptr;
      }

      if (myEmbreeDevice)
      {
        myEmbreeScene = rtcNewScene(myEmbreeDevice);

        // Create triangle geometry
        RTCGeometry geom = rtcNewGeometry(myEmbreeDevice, RTC_GEOMETRY_TYPE_TRIANGLE);

        // Set vertex buffer (Embree expects float, not double)
        float* verts = (float*)rtcSetNewGeometryBuffer(geom,
                                                       RTC_BUFFER_TYPE_VERTEX,
                                                       0,
                                                       RTC_FORMAT_FLOAT3,
                                                       3 * sizeof(float),
                                                       uniqueVertices.size());
        for (size_t i = 0; i < uniqueVertices.size(); ++i)
        {
          verts[3 * i + 0] = static_cast<float>(uniqueVertices[i][0]);
          verts[3 * i + 1] = static_cast<float>(uniqueVertices[i][1]);
          verts[3 * i + 2] = static_cast<float>(uniqueVertices[i][2]);
        }

        // Set index buffer
        unsigned* idxs = (unsigned*)rtcSetNewGeometryBuffer(geom,
                                                            RTC_BUFFER_TYPE_INDEX,
                                                            0,
                                                            RTC_FORMAT_UINT3,
                                                            3 * sizeof(unsigned),
                                                            nTriangles);
        for (Standard_Integer i = 0; i < nTriangles; ++i)
        {
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

void BRepIntCurveSurface_InterBVH::Perform(const gp_Lin&       theLine,
                                           const Standard_Real theMin,
                                           const Standard_Real theMax)
{
  myIsDone = Standard_False;
  myNbPnt  = 0;
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
  Standard_Integer hitTriIdx  = aTriTraverser.GetHitTriangleIndex();

  if (hitFaceIdx >= 0 && hitFaceIdx < static_cast<Standard_Integer>(mySurfaceAdaptors.size())
      && hitTriIdx >= 0 && hitTriIdx < static_cast<Standard_Integer>(myTriangleInfo.size()))
  {
    // Step 2: UV-guided Newton refinement
    const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[hitTriIdx];
    Standard_Real                           baryU, baryV;
    aTriTraverser.GetHitBarycentric(baryU, baryV);
    Standard_Real baryW = 1.0 - baryU - baryV;

    // Interpolate UV: UV = w*UV0 + u*UV1 + v*UV2
    Standard_Real initU =
      baryW * triInfo.UV0.X() + baryU * triInfo.UV1.X() + baryV * triInfo.UV2.X();
    Standard_Real initV =
      baryW * triInfo.UV0.Y() + baryU * triInfo.UV1.Y() + baryV * triInfo.UV2.Y();

    // Refine using Newton iteration (skip for planar faces)
    const BRepAdaptor_Surface& aSurface = *mySurfaceAdaptors[hitFaceIdx];
    Standard_Real hitT = aTriTraverser.GetHitT();
    Standard_Real finalU   = initU;
    Standard_Real finalV   = initV;
    Standard_Real finalT   = hitT;
    gp_Pnt        finalPnt = theLine.Location().Translated(hitT * gp_Vec(theLine.Direction()));
    Standard_Integer iterCount = 0;
    NewtonResult     newtonResult = NewtonResult::Converged;

    // OPTIMIZATION: Skip Newton refinement for planar faces
    Standard_Boolean isPlanar = mySkipNewtonForPlanar
                                && hitFaceIdx < static_cast<Standard_Integer>(myIsPlanarFace.size())
                                && myIsPlanarFace[hitFaceIdx];

    if (!isPlanar)
    {
      newtonResult = RefineIntersectionNewton(aSurface,
                                              theLine.Location(),
                                              theLine.Direction(),
                                              finalU,
                                              finalV,
                                              finalT,
                                              finalPnt,
                                              iterCount,
                                              myNewtonTolerance,
                                              myNewtonMaxIter);
    }

    if (hitT >= theMin && hitT <= theMax)
    {
      BRepIntCurveSurface_HitResult aResult;
      aResult.IsValid = Standard_True;

      if (isPlanar || (newtonResult == NewtonResult::Converged && finalT >= theMin && finalT <= theMax))
      {
        aResult.Point = finalPnt;
        aResult.U     = isPlanar ? initU : finalU;
        aResult.V     = isPlanar ? initV : finalV;
        aResult.W     = isPlanar ? hitT : finalT;
      }
      else
      {
        // Newton failed - use triangle intersection point directly
        aResult.Point = theLine.Location().Translated(hitT * gp_Vec(theLine.Direction()));
        aResult.U     = initU;
        aResult.V     = initV;
        aResult.W     = hitT;
      }

      aResult.FaceIndex  = hitFaceIdx + 1; // 1-based
      aResult.Transition = IntCurveSurface_In;
      aResult.State      = TopAbs_IN;

      // Compute surface normal (and optionally curvatures)
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv;
      gp_Vec normalVec;

      if (myComputeCurvature)
      {
        // Full D2 evaluation for curvatures
        gp_Vec d2Sdu2, d2Sdv2, d2Sduv;
        aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
          if (aFace.Orientation() == TopAbs_REVERSED)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);

          // Curvature computation
          Standard_Real E     = dSdu.Dot(dSdu);
          Standard_Real F     = dSdu.Dot(dSdv);
          Standard_Real G     = dSdv.Dot(dSdv);
          Standard_Real L     = d2Sdu2.Dot(normalVec);
          Standard_Real M     = d2Sduv.Dot(normalVec);
          Standard_Real N     = d2Sdv2.Dot(normalVec);
          Standard_Real denom = E * G - F * F;

          if (std::abs(denom) > 1e-20)
          {
            aResult.GaussianCurvature = (L * N - M * M) / denom;
            aResult.MeanCurvature     = (E * N - 2.0 * F * M + G * L) / (2.0 * denom);
            Standard_Real disc =
              aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
            Standard_Real sqrtDisc = std::sqrt(std::max(0.0, disc));
            aResult.MinCurvature   = aResult.MeanCurvature - sqrtDisc;
            aResult.MaxCurvature   = aResult.MeanCurvature + sqrtDisc;
          }
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
      }
      else
      {
        // D1 only - just compute normal (much cheaper than D2)
        aSurface.D1(aResult.U, aResult.V, normPnt, dSdu, dSdv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
          if (aFace.Orientation() == TopAbs_REVERSED)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
        // Curvatures remain at default 0
      }

      myResults.push_back(aResult);
      myNbPnt = 1;
    }
  }

  myIsDone = Standard_True;
}

//=================================================================================================
// SIMD Newton Batch Processing Helper
//=================================================================================================

namespace
{

//! Structure to hold a pending curved-surface hit for SIMD Newton batching
struct PendingCurvedHit
{
  Standard_Integer ResultIdx;   //!< Index into theResults array
  Standard_Integer FaceIdx;     //!< Face index (0-based)
  Standard_Integer TriIdx;      //!< Triangle index
  Standard_Real    HitT;        //!< Initial ray parameter
  Standard_Real    BaryU, BaryV;//!< Barycentric coordinates
  Standard_Real    InitU, InitV;//!< Initial UV from barycentric interpolation
  gp_Pnt           RayOrg;      //!< Ray origin
  gp_Dir           RayDir;      //!< Ray direction
  Standard_Integer USpanIndex;  //!< U knot span index (for B-spline surfaces)
  Standard_Integer VSpanIndex;  //!< V knot span index (for B-spline surfaces)
};

#ifdef OCCT_RT_HAS_AVX

//! Process a batch of 8 curved-surface hits using AVX SIMD Newton
//! All hits must be on the SAME face for this to work efficiently
void ProcessNewtonBatch8_AVX(
  const std::vector<PendingCurvedHit>& hits,
  size_t                               startIdx,
  size_t                               count,
  const Adaptor3d_Surface&             surface,
  Standard_Real                        uMin,
  Standard_Real                        uMax,
  Standard_Real                        vMin,
  Standard_Real                        vMax,
  Standard_Real                        tolerance,
  Standard_Integer                     maxIter,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& results)
{
  if (count == 0) return;

  NewtonBatch8 batch;
  batch.ValidCount = static_cast<int>(std::min(count, size_t(8)));
  batch.ActiveMask = (1u << batch.ValidCount) - 1;

  // Fill batch with hit data
  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    batch.U[i] = static_cast<float>(hit.InitU);
    batch.V[i] = static_cast<float>(hit.InitV);
    batch.RayOrgX[i] = static_cast<float>(hit.RayOrg.X());
    batch.RayOrgY[i] = static_cast<float>(hit.RayOrg.Y());
    batch.RayOrgZ[i] = static_cast<float>(hit.RayOrg.Z());
    batch.RayDirX[i] = static_cast<float>(hit.RayDir.X());
    batch.RayDirY[i] = static_cast<float>(hit.RayDir.Y());
    batch.RayDirZ[i] = static_cast<float>(hit.RayDir.Z());
    batch.UMin[i] = static_cast<float>(uMin);
    batch.UMax[i] = static_cast<float>(uMax);
    batch.VMin[i] = static_cast<float>(vMin);
    batch.VMax[i] = static_cast<float>(vMax);
  }

  // Pad remaining slots
  for (int i = batch.ValidCount; i < 8; ++i)
  {
    batch.U[i] = batch.U[0];
    batch.V[i] = batch.V[0];
    batch.RayOrgX[i] = batch.RayOrgX[0];
    batch.RayOrgY[i] = batch.RayOrgY[0];
    batch.RayOrgZ[i] = batch.RayOrgZ[0];
    batch.RayDirX[i] = batch.RayDirX[0];
    batch.RayDirY[i] = batch.RayDirY[0];
    batch.RayDirZ[i] = batch.RayDirZ[0];
    batch.UMin[i] = batch.UMin[0];
    batch.UMax[i] = batch.UMax[0];
    batch.VMin[i] = batch.VMin[0];
    batch.VMax[i] = batch.VMax[0];
  }

  // Create D1 evaluation callback
  auto surfaceD1 = [&](int slot, float u, float v,
                       float& Sx, float& Sy, float& Sz,
                       float& dSuX, float& dSuY, float& dSuZ,
                       float& dSvX, float& dSvY, float& dSvZ) {
    gp_Pnt S;
    gp_Vec dSu, dSv;
    surface.D1(static_cast<Standard_Real>(u), static_cast<Standard_Real>(v), S, dSu, dSv);
    Sx = static_cast<float>(S.X());
    Sy = static_cast<float>(S.Y());
    Sz = static_cast<float>(S.Z());
    dSuX = static_cast<float>(dSu.X());
    dSuY = static_cast<float>(dSu.Y());
    dSuZ = static_cast<float>(dSu.Z());
    dSvX = static_cast<float>(dSv.X());
    dSvY = static_cast<float>(dSv.Y());
    dSvZ = static_cast<float>(dSv.Z());
  };

  // Configure Newton
  NewtonBatchConfig config;
  config.Tolerance = static_cast<float>(tolerance);
  config.MaxIter = maxIter;
  config.RelaxedTol = static_cast<float>(tolerance * 100.0);

  // Run SIMD Newton
  NewtonIterateBatch8_AVX(batch, surfaceD1, config);

  // Extract results
  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    BRepIntCurveSurface_HitResult& result = results(hit.ResultIdx);

    if (batch.Converged[i] && batch.OutT[i] >= 0.0f)
    {
      result.Point = gp_Pnt(batch.OutPntX[i], batch.OutPntY[i], batch.OutPntZ[i]);
      result.U = static_cast<Standard_Real>(batch.OutU[i]);
      result.V = static_cast<Standard_Real>(batch.OutV[i]);
      result.W = static_cast<Standard_Real>(batch.OutT[i]);
    }
    else
    {
      // Newton failed - use triangle hit
      result.Point = hit.RayOrg.Translated(hit.HitT * gp_Vec(hit.RayDir));
      result.U = hit.InitU;
      result.V = hit.InitV;
      result.W = hit.HitT;
    }
  }
}

#endif // OCCT_RT_HAS_AVX

#ifdef OCCT_RT_HAS_SSE

//! Process a batch of 4 curved-surface hits using SSE SIMD Newton
void ProcessNewtonBatch4_SSE(
  const std::vector<PendingCurvedHit>& hits,
  size_t                               startIdx,
  size_t                               count,
  const Adaptor3d_Surface&             surface,
  Standard_Real                        uMin,
  Standard_Real                        uMax,
  Standard_Real                        vMin,
  Standard_Real                        vMax,
  Standard_Real                        tolerance,
  Standard_Integer                     maxIter,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& results)
{
  if (count == 0) return;

  NewtonBatch4 batch;
  batch.ValidCount = static_cast<int>(std::min(count, size_t(4)));
  batch.ActiveMask = (1u << batch.ValidCount) - 1;

  // Fill batch
  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    batch.U[i] = static_cast<float>(hit.InitU);
    batch.V[i] = static_cast<float>(hit.InitV);
    batch.RayOrgX[i] = static_cast<float>(hit.RayOrg.X());
    batch.RayOrgY[i] = static_cast<float>(hit.RayOrg.Y());
    batch.RayOrgZ[i] = static_cast<float>(hit.RayOrg.Z());
    batch.RayDirX[i] = static_cast<float>(hit.RayDir.X());
    batch.RayDirY[i] = static_cast<float>(hit.RayDir.Y());
    batch.RayDirZ[i] = static_cast<float>(hit.RayDir.Z());
    batch.UMin[i] = static_cast<float>(uMin);
    batch.UMax[i] = static_cast<float>(uMax);
    batch.VMin[i] = static_cast<float>(vMin);
    batch.VMax[i] = static_cast<float>(vMax);
  }

  // Pad remaining
  for (int i = batch.ValidCount; i < 4; ++i)
  {
    batch.U[i] = batch.U[0];
    batch.V[i] = batch.V[0];
    batch.RayOrgX[i] = batch.RayOrgX[0];
    batch.RayOrgY[i] = batch.RayOrgY[0];
    batch.RayOrgZ[i] = batch.RayOrgZ[0];
    batch.RayDirX[i] = batch.RayDirX[0];
    batch.RayDirY[i] = batch.RayDirY[0];
    batch.RayDirZ[i] = batch.RayDirZ[0];
    batch.UMin[i] = batch.UMin[0];
    batch.UMax[i] = batch.UMax[0];
    batch.VMin[i] = batch.VMin[0];
    batch.VMax[i] = batch.VMax[0];
  }

  auto surfaceD1 = [&](int slot, float u, float v,
                       float& Sx, float& Sy, float& Sz,
                       float& dSuX, float& dSuY, float& dSuZ,
                       float& dSvX, float& dSvY, float& dSvZ) {
    gp_Pnt S;
    gp_Vec dSu, dSv;
    surface.D1(static_cast<Standard_Real>(u), static_cast<Standard_Real>(v), S, dSu, dSv);
    Sx = static_cast<float>(S.X());
    Sy = static_cast<float>(S.Y());
    Sz = static_cast<float>(S.Z());
    dSuX = static_cast<float>(dSu.X());
    dSuY = static_cast<float>(dSu.Y());
    dSuZ = static_cast<float>(dSu.Z());
    dSvX = static_cast<float>(dSv.X());
    dSvY = static_cast<float>(dSv.Y());
    dSvZ = static_cast<float>(dSv.Z());
  };

  NewtonBatchConfig config;
  config.Tolerance = static_cast<float>(tolerance);
  config.MaxIter = maxIter;
  config.RelaxedTol = static_cast<float>(tolerance * 100.0);

  NewtonIterateBatch4_SSE(batch, surfaceD1, config);

  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    BRepIntCurveSurface_HitResult& result = results(hit.ResultIdx);

    if (batch.Converged[i] && batch.OutT[i] >= 0.0f)
    {
      result.Point = gp_Pnt(batch.OutPntX[i], batch.OutPntY[i], batch.OutPntZ[i]);
      result.U = static_cast<Standard_Real>(batch.OutU[i]);
      result.V = static_cast<Standard_Real>(batch.OutV[i]);
      result.W = static_cast<Standard_Real>(batch.OutT[i]);
    }
    else
    {
      result.Point = hit.RayOrg.Translated(hit.HitT * gp_Vec(hit.RayDir));
      result.U = hit.InitU;
      result.V = hit.InitV;
      result.W = hit.HitT;
    }
  }
}

#endif // OCCT_RT_HAS_SSE

#ifdef OCCT_RT_HAS_NEON

//! Pre-computed Bézier patch data (control points for one patch)
struct BezierPatchData
{
  std::vector<float> CtrlX;  //!< Control point X coordinates [(degU+1)*(degV+1)]
  std::vector<float> CtrlY;  //!< Control point Y coordinates
  std::vector<float> CtrlZ;  //!< Control point Z coordinates
  Standard_Real UMin, UMax;  //!< Parameter bounds in U
  Standard_Real VMin, VMax;  //!< Parameter bounds in V
  Standard_Integer DegU;     //!< Degree in U
  Standard_Integer DegV;     //!< Degree in V
};

//! Pre-computed data for B-spline surface evaluation (reused across batches)
struct BSplineCacheData
{
  Handle(Geom_BSplineSurface) BSpline;
  Handle(BSplSLib_Cache)      Cache;
  TColStd_Array1OfReal        UFlatKnots;
  TColStd_Array1OfReal        VFlatKnots;
  const TColStd_Array2OfReal* Weights;
  Standard_Integer            CachedUSpan;  //!< Currently cached U span index
  Standard_Integer            CachedVSpan;  //!< Currently cached V span index

  //! Pre-computed Bézier patches (grid of patches)
  std::vector<std::vector<BezierPatchData>> BezierPatches;  //!< [uPatch][vPatch]
  std::vector<Standard_Real> UKnots;  //!< U knot values (patch boundaries)
  std::vector<Standard_Real> VKnots;  //!< V knot values (patch boundaries)
  Standard_Integer NbUPatches;
  Standard_Integer NbVPatches;
  bool BezierPatchesReady;

  BSplineCacheData() : Weights(nullptr), UFlatKnots(1, 1), VFlatKnots(1, 1),
                       CachedUSpan(-1), CachedVSpan(-1), NbUPatches(0), NbVPatches(0),
                       BezierPatchesReady(false) {}

  //! Initialize from B-spline surface
  void Init(const Handle(Geom_BSplineSurface)& theBSpline)
  {
    BSpline = theBSpline;
    CachedUSpan = -1;
    CachedVSpan = -1;
    BezierPatchesReady = false;
    if (BSpline.IsNull()) return;

    // Build flat knot arrays once
    Standard_Integer nUFlat = BSpline->NbUPoles() + BSpline->UDegree() + 1;
    Standard_Integer nVFlat = BSpline->NbVPoles() + BSpline->VDegree() + 1;

    UFlatKnots = TColStd_Array1OfReal(1, nUFlat);
    VFlatKnots = TColStd_Array1OfReal(1, nVFlat);

    Standard_Integer idx = 1;
    for (Standard_Integer i = 1; i <= BSpline->NbUKnots(); ++i)
    {
      for (Standard_Integer j = 0; j < BSpline->UMultiplicity(i); ++j)
      {
        UFlatKnots(idx++) = BSpline->UKnot(i);
      }
    }

    idx = 1;
    for (Standard_Integer i = 1; i <= BSpline->NbVKnots(); ++i)
    {
      for (Standard_Integer j = 0; j < BSpline->VMultiplicity(i); ++j)
      {
        VFlatKnots(idx++) = BSpline->VKnot(i);
      }
    }

    Weights = BSpline->Weights();

    // Create cache for Newton fallback
    Cache = new BSplSLib_Cache(
      BSpline->UDegree(), BSpline->IsUPeriodic(), UFlatKnots,
      BSpline->VDegree(), BSpline->IsVPeriodic(), VFlatKnots,
      Weights);

    // Pre-compute Bézier patches using GeomConvert
    // This converts the B-spline into a grid of Bézier patches via knot insertion
    try
    {
      GeomConvert_BSplineSurfaceToBezierSurface converter(BSpline);

      NbUPatches = converter.NbUPatches();
      NbVPatches = converter.NbVPatches();

      // Get knot values (patch boundaries)
      TColStd_Array1OfReal uKnots(1, NbUPatches + 1);
      TColStd_Array1OfReal vKnots(1, NbVPatches + 1);
      converter.UKnots(uKnots);
      converter.VKnots(vKnots);

      UKnots.resize(NbUPatches + 1);
      VKnots.resize(NbVPatches + 1);
      for (Standard_Integer i = 1; i <= NbUPatches + 1; ++i)
        UKnots[i - 1] = uKnots(i);
      for (Standard_Integer i = 1; i <= NbVPatches + 1; ++i)
        VKnots[i - 1] = vKnots(i);

      // Extract all Bézier patches
      BezierPatches.resize(NbUPatches);
      for (Standard_Integer ui = 1; ui <= NbUPatches; ++ui)
      {
        BezierPatches[ui - 1].resize(NbVPatches);
        for (Standard_Integer vi = 1; vi <= NbVPatches; ++vi)
        {
          Handle(Geom_BezierSurface) bezier = converter.Patch(ui, vi);
          BezierPatchData& patch = BezierPatches[ui - 1][vi - 1];

          patch.DegU = bezier->UDegree();
          patch.DegV = bezier->VDegree();
          patch.UMin = UKnots[ui - 1];
          patch.UMax = UKnots[ui];
          patch.VMin = VKnots[vi - 1];
          patch.VMax = VKnots[vi];

          // Copy control points
          const TColgp_Array2OfPnt& poles = bezier->Poles();
          Standard_Integer nU = poles.RowLength();  // = degU + 1
          Standard_Integer nV = poles.ColLength();  // = degV + 1
          Standard_Integer nPts = nU * nV;

          patch.CtrlX.resize(nPts);
          patch.CtrlY.resize(nPts);
          patch.CtrlZ.resize(nPts);

          // Store in row-major order: [v][u]
          for (Standard_Integer vj = poles.LowerCol(); vj <= poles.UpperCol(); ++vj)
          {
            for (Standard_Integer ui2 = poles.LowerRow(); ui2 <= poles.UpperRow(); ++ui2)
            {
              const gp_Pnt& P = poles(ui2, vj);
              Standard_Integer localU = ui2 - poles.LowerRow();
              Standard_Integer localV = vj - poles.LowerCol();
              Standard_Integer idx2 = localV * nU + localU;

              patch.CtrlX[idx2] = static_cast<float>(P.X());
              patch.CtrlY[idx2] = static_cast<float>(P.Y());
              patch.CtrlZ[idx2] = static_cast<float>(P.Z());
            }
          }
        }
      }

      BezierPatchesReady = true;
      // std::cerr << "[BSplineCache] Pre-computed " << NbUPatches << "x" << NbVPatches
      //           << " Bézier patches" << std::endl;
    }
    catch (...)
    {
      std::cerr << "[BSplineCache] Warning: Failed to convert to Bézier patches" << std::endl;
      BezierPatchesReady = false;
    }
  }

  //! Build cache for specific knot span (using pre-computed span indices)
  //! This avoids the binary search in IsCacheValid
  void BuildCacheForSpan(Standard_Integer uSpan, Standard_Integer vSpan)
  {
    if (uSpan == CachedUSpan && vSpan == CachedVSpan)
      return;  // Already cached

    // Get a UV point inside the span
    // Use midpoint of the span's knot interval
    Standard_Real u = (uSpan >= 1 && uSpan < BSpline->NbUKnots())
      ? 0.5 * (BSpline->UKnot(uSpan) + BSpline->UKnot(uSpan + 1))
      : BSpline->UKnot(1);
    Standard_Real v = (vSpan >= 1 && vSpan < BSpline->NbVKnots())
      ? 0.5 * (BSpline->VKnot(vSpan) + BSpline->VKnot(vSpan + 1))
      : BSpline->VKnot(1);

    Cache->BuildCache(u, v, UFlatKnots, VFlatKnots, BSpline->Poles(), Weights);
    CachedUSpan = uSpan;
    CachedVSpan = vSpan;
  }

  //! Evaluate D1 with automatic cache management
  void D1(Standard_Real u, Standard_Real v, gp_Pnt& S, gp_Vec& dSu, gp_Vec& dSv)
  {
    if (!Cache->IsCacheValid(u, v))
    {
      Cache->BuildCache(u, v, UFlatKnots, VFlatKnots, BSpline->Poles(), Weights);
      CachedUSpan = -1;  // Mark as unknown span
      CachedVSpan = -1;
    }
    Cache->D1(u, v, S, dSu, dSv);
  }

  //! Evaluate D1 assuming cache is already valid for this span (no validity check!)
  //! Use only when you're sure the UV is in the cached span
  void D1_NoCheck(Standard_Real u, Standard_Real v, gp_Pnt& S, gp_Vec& dSu, gp_Vec& dSv)
  {
    Cache->D1(u, v, S, dSu, dSv);
  }

  //! Find the Bézier patch containing (u, v) and return its index
  //! @param u Parameter in U direction
  //! @param v Parameter in V direction
  //! @param uPatchIdx Output: U patch index (0-based)
  //! @param vPatchIdx Output: V patch index (0-based)
  //! @return true if patch found
  bool FindBezierPatch(Standard_Real u, Standard_Real v,
                       Standard_Integer& uPatchIdx, Standard_Integer& vPatchIdx) const
  {
    if (!BezierPatchesReady || NbUPatches == 0 || NbVPatches == 0)
      return false;

    // Binary search in U direction
    uPatchIdx = 0;
    for (Standard_Integer i = 0; i < NbUPatches; ++i)
    {
      if (u >= UKnots[i] && u <= UKnots[i + 1])
      {
        uPatchIdx = i;
        break;
      }
    }

    // Binary search in V direction
    vPatchIdx = 0;
    for (Standard_Integer i = 0; i < NbVPatches; ++i)
    {
      if (v >= VKnots[i] && v <= VKnots[i + 1])
      {
        vPatchIdx = i;
        break;
      }
    }

    return true;
  }

  //! Extract local Bézier control points for the patch containing (u, v)
  //! Uses pre-computed Bézier patches from GeomConvert (exact control points!)
  //! Control points are output in row-major order: [v][u] where v varies slowest
  //! @param u Parameter in U direction
  //! @param v Parameter in V direction
  //! @param ctrlPtsX Output X coordinates of control points [(degU+1)*(degV+1)]
  //! @param ctrlPtsY Output Y coordinates
  //! @param ctrlPtsZ Output Z coordinates
  //! @param spanUMin Output: U minimum of the patch
  //! @param spanUMax Output: U maximum of the patch
  //! @param spanVMin Output: V minimum of the patch
  //! @param spanVMax Output: V maximum of the patch
  //! @param degU Output: Degree in U
  //! @param degV Output: Degree in V
  //! @return true if extraction successful
  bool ExtractBezierPatch(Standard_Real u, Standard_Real v,
                          float* ctrlPtsX, float* ctrlPtsY, float* ctrlPtsZ,
                          Standard_Real& spanUMin, Standard_Real& spanUMax,
                          Standard_Real& spanVMin, Standard_Real& spanVMax,
                          Standard_Integer& degU, Standard_Integer& degV)
  {
    if (!BezierPatchesReady)
      return false;

    Standard_Integer uPatchIdx, vPatchIdx;
    if (!FindBezierPatch(u, v, uPatchIdx, vPatchIdx))
      return false;

    const BezierPatchData& patch = BezierPatches[uPatchIdx][vPatchIdx];

    spanUMin = patch.UMin;
    spanUMax = patch.UMax;
    spanVMin = patch.VMin;
    spanVMax = patch.VMax;
    degU = patch.DegU;
    degV = patch.DegV;

    // Copy control points
    Standard_Integer nPts = (degU + 1) * (degV + 1);
    std::copy(patch.CtrlX.begin(), patch.CtrlX.begin() + nPts, ctrlPtsX);
    std::copy(patch.CtrlY.begin(), patch.CtrlY.begin() + nPts, ctrlPtsY);
    std::copy(patch.CtrlZ.begin(), patch.CtrlZ.begin() + nPts, ctrlPtsZ);

    return true;
  }

  //! Backward-compatible version (uses B-spline degrees)
  bool ExtractBezierPatch(Standard_Real u, Standard_Real v,
                          float* ctrlPtsX, float* ctrlPtsY, float* ctrlPtsZ,
                          Standard_Real& spanUMin, Standard_Real& spanUMax,
                          Standard_Real& spanVMin, Standard_Real& spanVMax)
  {
    Standard_Integer degU, degV;
    return ExtractBezierPatch(u, v, ctrlPtsX, ctrlPtsY, ctrlPtsZ,
                              spanUMin, spanUMax, spanVMin, spanVMax, degU, degV);
  }
};

//! Process a batch of 4 curved-surface hits using ARM NEON SIMD Newton
//! Uses fully vectorized D1 evaluation for elementary surfaces (torus, cylinder, sphere)
//! For sphere/cylinder: uses ANALYTIC intersection (no Newton iterations!)
//! For B-spline: uses BSplSLib_Cache for faster evaluation
//! @param bsplineCache Optional pre-computed B-spline cache data (created once per face)
void ProcessNewtonBatch4_NEON(
  const std::vector<PendingCurvedHit>& hits,
  size_t                               startIdx,
  size_t                               count,
  const Adaptor3d_Surface&             surface,
  Standard_Real                        uMin,
  Standard_Real                        uMax,
  Standard_Real                        vMin,
  Standard_Real                        vMax,
  Standard_Real                        tolerance,
  Standard_Integer                     maxIter,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& results,
  BSplineCacheData*                    bsplineCache = nullptr)
{
  if (count == 0) return;

  GeomAbs_SurfaceType surfType = surface.GetType();

  // For plane, sphere, cylinder, cone, and torus: use ANALYTIC intersection (skip Newton entirely!)
  if (surfType == GeomAbs_Plane || surfType == GeomAbs_Sphere || surfType == GeomAbs_Cylinder ||
      surfType == GeomAbs_Cone || surfType == GeomAbs_Torus)
  {
    SIMDSurfaceParams surfParams;
    if (surfType == GeomAbs_Plane)
    {
      gp_Pln pln = surface.Plane();
      surfParams.Init(pln.Position(), GeomAbs_Plane);
    }
    else if (surfType == GeomAbs_Sphere)
    {
      gp_Sphere sph = surface.Sphere();
      surfParams.Init(sph.Position(), GeomAbs_Sphere, sph.Radius());
    }
    else if (surfType == GeomAbs_Cylinder)
    {
      gp_Cylinder cyl = surface.Cylinder();
      surfParams.Init(cyl.Position(), GeomAbs_Cylinder, cyl.Radius());
    }
    else if (surfType == GeomAbs_Cone)
    {
      gp_Cone cone = surface.Cone();
      surfParams.Init(cone.Position(), GeomAbs_Cone, cone.RefRadius(), 0.0, 0.0, cone.SemiAngle());
    }
    else // Torus
    {
      gp_Torus torus = surface.Torus();
      surfParams.Init(torus.Position(), GeomAbs_Torus, 0.0, torus.MajorRadius(), torus.MinorRadius());
    }

    for (size_t i = 0; i < count; ++i)
    {
      const PendingCurvedHit& hit = hits[startIdx + i];
      BRepIntCurveSurface_HitResult& aResult = results(hit.ResultIdx);

      float outT, outU, outV;
      bool found = false;

      if (surfType == GeomAbs_Plane)
      {
        found = AnalyticPlaneIntersect(
          surfParams,
          static_cast<float>(hit.RayOrg.X()),
          static_cast<float>(hit.RayOrg.Y()),
          static_cast<float>(hit.RayOrg.Z()),
          static_cast<float>(hit.RayDir.X()),
          static_cast<float>(hit.RayDir.Y()),
          static_cast<float>(hit.RayDir.Z()),
          0.0f, 1e30f,
          outT, outU, outV);
      }
      else if (surfType == GeomAbs_Sphere)
      {
        found = AnalyticSphereIntersect(
          surfParams,
          static_cast<float>(hit.RayOrg.X()),
          static_cast<float>(hit.RayOrg.Y()),
          static_cast<float>(hit.RayOrg.Z()),
          static_cast<float>(hit.RayDir.X()),
          static_cast<float>(hit.RayDir.Y()),
          static_cast<float>(hit.RayDir.Z()),
          0.0f, 1e30f,
          outT, outU, outV);
      }
      else if (surfType == GeomAbs_Cylinder)
      {
        found = AnalyticCylinderIntersect(
          surfParams,
          static_cast<float>(hit.RayOrg.X()),
          static_cast<float>(hit.RayOrg.Y()),
          static_cast<float>(hit.RayOrg.Z()),
          static_cast<float>(hit.RayDir.X()),
          static_cast<float>(hit.RayDir.Y()),
          static_cast<float>(hit.RayDir.Z()),
          0.0f, 1e30f,
          static_cast<float>(vMin), static_cast<float>(vMax),
          outT, outU, outV);
      }
      else if (surfType == GeomAbs_Cone)
      {
        found = AnalyticConeIntersect(
          surfParams,
          static_cast<float>(hit.RayOrg.X()),
          static_cast<float>(hit.RayOrg.Y()),
          static_cast<float>(hit.RayOrg.Z()),
          static_cast<float>(hit.RayDir.X()),
          static_cast<float>(hit.RayDir.Y()),
          static_cast<float>(hit.RayDir.Z()),
          0.0f, 1e30f,
          static_cast<float>(vMin), static_cast<float>(vMax),
          outT, outU, outV);
      }
      else // Torus - use Ferrari's quartic solver
      {
        found = AnalyticTorusIntersect(
          surfParams,
          static_cast<float>(hit.RayOrg.X()),
          static_cast<float>(hit.RayOrg.Y()),
          static_cast<float>(hit.RayOrg.Z()),
          static_cast<float>(hit.RayDir.X()),
          static_cast<float>(hit.RayDir.Y()),
          static_cast<float>(hit.RayDir.Z()),
          0.0f, 1e30f,
          outT, outU, outV);
      }

      if (found)
      {
        aResult.U = outU;
        aResult.V = outV;
        aResult.W = outT;
        gp_Pnt hitPnt(
          hit.RayOrg.X() + outT * hit.RayDir.X(),
          hit.RayOrg.Y() + outT * hit.RayDir.Y(),
          hit.RayOrg.Z() + outT * hit.RayDir.Z());
        aResult.Point = hitPnt;
      }
      else
      {
        // Fallback to triangle intersection if analytic fails
        aResult.U = hit.InitU;
        aResult.V = hit.InitV;
        aResult.W = hit.HitT;
        gp_Pnt hitPnt(
          hit.RayOrg.X() + hit.HitT * hit.RayDir.X(),
          hit.RayOrg.Y() + hit.HitT * hit.RayDir.Y(),
          hit.RayOrg.Z() + hit.HitT * hit.RayDir.Z());
        aResult.Point = hitPnt;
      }
    }
    return;
  }

  // For torus and other surfaces: use Newton iteration
  NewtonBatch4 batch;
  batch.ValidCount = static_cast<int>(std::min(count, size_t(4)));
  batch.ActiveMask = (1u << batch.ValidCount) - 1;

  // Fill batch
  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    batch.U[i] = static_cast<float>(hit.InitU);
    batch.V[i] = static_cast<float>(hit.InitV);
    batch.RayOrgX[i] = static_cast<float>(hit.RayOrg.X());
    batch.RayOrgY[i] = static_cast<float>(hit.RayOrg.Y());
    batch.RayOrgZ[i] = static_cast<float>(hit.RayOrg.Z());
    batch.RayDirX[i] = static_cast<float>(hit.RayDir.X());
    batch.RayDirY[i] = static_cast<float>(hit.RayDir.Y());
    batch.RayDirZ[i] = static_cast<float>(hit.RayDir.Z());
    batch.UMin[i] = static_cast<float>(uMin);
    batch.UMax[i] = static_cast<float>(uMax);
    batch.VMin[i] = static_cast<float>(vMin);
    batch.VMax[i] = static_cast<float>(vMax);
  }

  // Pad remaining
  for (int i = batch.ValidCount; i < 4; ++i)
  {
    batch.U[i] = batch.U[0];
    batch.V[i] = batch.V[0];
    batch.RayOrgX[i] = batch.RayOrgX[0];
    batch.RayOrgY[i] = batch.RayOrgY[0];
    batch.RayOrgZ[i] = batch.RayOrgZ[0];
    batch.RayDirX[i] = batch.RayDirX[0];
    batch.RayDirY[i] = batch.RayDirY[0];
    batch.RayDirZ[i] = batch.RayDirZ[0];
    batch.UMin[i] = batch.UMin[0];
    batch.UMax[i] = batch.UMax[0];
    batch.VMin[i] = batch.VMin[0];
    batch.VMax[i] = batch.VMax[0];
  }

  NewtonBatchConfig config;
  config.Tolerance = static_cast<float>(tolerance);
  config.MaxIter = maxIter;
  config.RelaxedTol = static_cast<float>(tolerance * 100.0);

  // Check if surface type supports fully vectorized D1
  bool useSIMDSurface = SupportsSIMDSurface(surfType);

  if (useSIMDSurface)
  {
    // Extract surface parameters for SIMD evaluation
    SIMDSurfaceParams surfParams;

    switch (surfType)
    {
      case GeomAbs_Torus:
      {
        gp_Torus torus = surface.Torus();
        surfParams.Init(torus.Position(), GeomAbs_Torus,
                        0.0, torus.MajorRadius(), torus.MinorRadius());
        break;
      }
      case GeomAbs_Cylinder:
      {
        gp_Cylinder cyl = surface.Cylinder();
        surfParams.Init(cyl.Position(), GeomAbs_Cylinder, cyl.Radius());
        break;
      }
      case GeomAbs_Sphere:
      {
        gp_Sphere sph = surface.Sphere();
        surfParams.Init(sph.Position(), GeomAbs_Sphere, sph.Radius());
        break;
      }
      default:
        useSIMDSurface = false;
        break;
    }

    if (useSIMDSurface)
    {
      // Create SIMD D1 evaluator lambda
      auto simdSurfD1 = [&surfParams](const float* u, const float* v,
                                       float* Px, float* Py, float* Pz,
                                       float* dSuX, float* dSuY, float* dSuZ,
                                       float* dSvX, float* dSvY, float* dSvZ) {
        SurfaceD1_4(surfParams, u, v, Px, Py, Pz, dSuX, dSuY, dSuZ, dSvX, dSvY, dSvZ);
      };

      // Scalar fallback (not used by this implementation, but required by API)
      auto scalarD1 = [&surface](int, float u, float v,
                                  float& Sx, float& Sy, float& Sz,
                                  float& dSuX, float& dSuY, float& dSuZ,
                                  float& dSvX, float& dSvY, float& dSvZ) {
        gp_Pnt S;
        gp_Vec dSu, dSv;
        surface.D1(u, v, S, dSu, dSv);
        Sx = static_cast<float>(S.X()); Sy = static_cast<float>(S.Y()); Sz = static_cast<float>(S.Z());
        dSuX = static_cast<float>(dSu.X()); dSuY = static_cast<float>(dSu.Y()); dSuZ = static_cast<float>(dSu.Z());
        dSvX = static_cast<float>(dSv.X()); dSvY = static_cast<float>(dSv.Y()); dSvZ = static_cast<float>(dSv.Z());
      };

      NewtonIterateBatch4_NEON_SIMDSurf(batch, simdSurfD1, scalarD1, config);
    }
  }

  if (!useSIMDSurface)
  {
    // Check for B-spline surface with pre-computed cache - use Bézier clipping
    if (surfType == GeomAbs_BSplineSurface && bsplineCache != nullptr && !bsplineCache->BSpline.IsNull())
    {
      Standard_Integer degU = bsplineCache->BSpline->UDegree();
      Standard_Integer degV = bsplineCache->BSpline->VDegree();

      // Skip Bézier clipping - go straight to Newton with custom SIMD D1
      // This gives better performance than Bézier clipping for typical cases
      for (int i = 0; i < batch.ValidCount; ++i)
      {
        batch.Converged[i] = false;
      }

      bool needNewton = true;

      if (needNewton)
      {
        // Only process unconverged hits
        NewtonBatch4 newtonBatch = batch;
        for (int i = 0; i < batch.ValidCount; ++i)
        {
          if (batch.Converged[i])
          {
            newtonBatch.ActiveMask &= ~(1u << i);  // Skip converged hits
          }
        }

        if (newtonBatch.ActiveMask != 0)
        {
          // Check if we can use custom SIMD D1 for bicubic patches
          bool useSIMDBezierD1 = (bsplineCache->BezierPatchesReady &&
                                   degU == 3 && degV == 3 &&
                                   bsplineCache->NbUPatches > 0);

          if (useSIMDBezierD1)
          {
            // Build float knot arrays for FindPatchIndex
            std::vector<float> uKnotsF(bsplineCache->UKnots.size());
            std::vector<float> vKnotsF(bsplineCache->VKnots.size());
            for (size_t ki = 0; ki < bsplineCache->UKnots.size(); ++ki)
              uKnotsF[ki] = static_cast<float>(bsplineCache->UKnots[ki]);
            for (size_t ki = 0; ki < bsplineCache->VKnots.size(); ++ki)
              vKnotsF[ki] = static_cast<float>(bsplineCache->VKnots[ki]);

            // Create SIMD D1 evaluator for bicubic Bézier patches
            auto simdBezierD1 = [bsplineCache, &uKnotsF, &vKnotsF](
                                  const float* u, const float* v,
                                  float* Px, float* Py, float* Pz,
                                  float* dSuX, float* dSuY, float* dSuZ,
                                  float* dSvX, float* dSvY, float* dSvZ) {
              // Check if all 4 points are in the same patch
              int uPatch0 = FindPatchIndex(u[0], uKnotsF.data(), static_cast<int>(uKnotsF.size()));
              int vPatch0 = FindPatchIndex(v[0], vKnotsF.data(), static_cast<int>(vKnotsF.size()));

              bool samePatch = true;
              for (int k = 1; k < 4; ++k)
              {
                int uPatchK = FindPatchIndex(u[k], uKnotsF.data(), static_cast<int>(uKnotsF.size()));
                int vPatchK = FindPatchIndex(v[k], vKnotsF.data(), static_cast<int>(vKnotsF.size()));
                if (uPatchK != uPatch0 || vPatchK != vPatch0)
                {
                  samePatch = false;
                  break;
                }
              }

              if (samePatch)
              {
                // Fast path: all 4 points in same patch - use SIMD
                const BezierPatchData& patch = bsplineCache->BezierPatches[uPatch0][vPatch0];

                // Convert global UV to local [0,1] coordinates
                float uSpan = static_cast<float>(patch.UMax - patch.UMin);
                float vSpan = static_cast<float>(patch.VMax - patch.VMin);
                float invUSpan = 1.0f / uSpan;
                float invVSpan = 1.0f / vSpan;

                alignas(16) float localU[4], localV[4];
                for (int k = 0; k < 4; ++k)
                {
                  localU[k] = (u[k] - static_cast<float>(patch.UMin)) * invUSpan;
                  localV[k] = (v[k] - static_cast<float>(patch.VMin)) * invVSpan;
                  // Clamp to [0,1]
                  localU[k] = std::max(0.0f, std::min(1.0f, localU[k]));
                  localV[k] = std::max(0.0f, std::min(1.0f, localV[k]));
                }

                // Evaluate using custom SIMD Bézier D1
                BezierD1_NEON4(
                  patch.CtrlX.data(), patch.CtrlY.data(), patch.CtrlZ.data(),
                  localU, localV,
                  Px, Py, Pz,
                  dSuX, dSuY, dSuZ,
                  dSvX, dSvY, dSvZ);

                // Scale derivatives by parameter span (chain rule)
                for (int k = 0; k < 4; ++k)
                {
                  dSuX[k] *= invUSpan;
                  dSuY[k] *= invUSpan;
                  dSuZ[k] *= invUSpan;
                  dSvX[k] *= invVSpan;
                  dSvY[k] *= invVSpan;
                  dSvZ[k] *= invVSpan;
                }
              }
              else
              {
                // Slow path: different patches - evaluate one at a time
                for (int k = 0; k < 4; ++k)
                {
                  int uPatchK = FindPatchIndex(u[k], uKnotsF.data(), static_cast<int>(uKnotsF.size()));
                  int vPatchK = FindPatchIndex(v[k], vKnotsF.data(), static_cast<int>(vKnotsF.size()));
                  const BezierPatchData& patch = bsplineCache->BezierPatches[uPatchK][vPatchK];

                  float uSpan = static_cast<float>(patch.UMax - patch.UMin);
                  float vSpan = static_cast<float>(patch.VMax - patch.VMin);
                  float invUSpan = 1.0f / uSpan;
                  float invVSpan = 1.0f / vSpan;

                  float localU = (u[k] - static_cast<float>(patch.UMin)) * invUSpan;
                  float localV = (v[k] - static_cast<float>(patch.VMin)) * invVSpan;
                  localU = std::max(0.0f, std::min(1.0f, localU));
                  localV = std::max(0.0f, std::min(1.0f, localV));

                  // Single-point evaluation using SIMD function (padding with same values)
                  alignas(16) float uArr[4] = {localU, localU, localU, localU};
                  alignas(16) float vArr[4] = {localV, localV, localV, localV};
                  alignas(16) float outPx[4], outPy[4], outPz[4];
                  alignas(16) float outDuX[4], outDuY[4], outDuZ[4];
                  alignas(16) float outDvX[4], outDvY[4], outDvZ[4];

                  BezierD1_NEON4(
                    patch.CtrlX.data(), patch.CtrlY.data(), patch.CtrlZ.data(),
                    uArr, vArr,
                    outPx, outPy, outPz,
                    outDuX, outDuY, outDuZ,
                    outDvX, outDvY, outDvZ);

                  Px[k] = outPx[0];
                  Py[k] = outPy[0];
                  Pz[k] = outPz[0];
                  dSuX[k] = outDuX[0] * invUSpan;
                  dSuY[k] = outDuY[0] * invUSpan;
                  dSuZ[k] = outDuZ[0] * invUSpan;
                  dSvX[k] = outDvX[0] * invVSpan;
                  dSvY[k] = outDvY[0] * invVSpan;
                  dSvZ[k] = outDvZ[0] * invVSpan;
                }
              }
            };

            // Scalar fallback (rarely used, but required by API)
            auto scalarD1 = [bsplineCache](int, float u, float v,
                               float& Sx, float& Sy, float& Sz,
                               float& dSuX, float& dSuY, float& dSuZ,
                               float& dSvX, float& dSvY, float& dSvZ) {
              gp_Pnt S;
              gp_Vec dSu, dSv;
              bsplineCache->D1(static_cast<Standard_Real>(u), static_cast<Standard_Real>(v), S, dSu, dSv);
              Sx = static_cast<float>(S.X());
              Sy = static_cast<float>(S.Y());
              Sz = static_cast<float>(S.Z());
              dSuX = static_cast<float>(dSu.X());
              dSuY = static_cast<float>(dSu.Y());
              dSuZ = static_cast<float>(dSu.Z());
              dSvX = static_cast<float>(dSv.X());
              dSvY = static_cast<float>(dSv.Y());
              dSvZ = static_cast<float>(dSv.Z());
            };

            NewtonIterateBatch4_NEON_SIMDSurf(newtonBatch, simdBezierD1, scalarD1, config);
          }
          else
          {
            // Fallback to OCCT's BSplSLib_Cache D1 for non-bicubic surfaces
            auto bsplineD1 = [bsplineCache](int, float u, float v,
                                 float& Sx, float& Sy, float& Sz,
                                 float& dSuX, float& dSuY, float& dSuZ,
                                 float& dSvX, float& dSvY, float& dSvZ) {
              gp_Pnt S;
              gp_Vec dSu, dSv;
              bsplineCache->D1(static_cast<Standard_Real>(u), static_cast<Standard_Real>(v), S, dSu, dSv);
              Sx = static_cast<float>(S.X());
              Sy = static_cast<float>(S.Y());
              Sz = static_cast<float>(S.Z());
              dSuX = static_cast<float>(dSu.X());
              dSuY = static_cast<float>(dSu.Y());
              dSuZ = static_cast<float>(dSu.Z());
              dSvX = static_cast<float>(dSv.X());
              dSvY = static_cast<float>(dSv.Y());
              dSvZ = static_cast<float>(dSv.Z());
            };

            NewtonIterateBatch4_NEON(newtonBatch, bsplineD1, config);
          }

          // Copy back results for unconverged hits
          for (int i = 0; i < batch.ValidCount; ++i)
          {
            if (!batch.Converged[i])
            {
              batch.OutU[i] = newtonBatch.OutU[i];
              batch.OutV[i] = newtonBatch.OutV[i];
              batch.OutT[i] = newtonBatch.OutT[i];
              batch.OutPntX[i] = newtonBatch.OutPntX[i];
              batch.OutPntY[i] = newtonBatch.OutPntY[i];
              batch.OutPntZ[i] = newtonBatch.OutPntZ[i];
              batch.Converged[i] = newtonBatch.Converged[i];
            }
          }
        }
      }
    }
    else
    {
      // Generic scalar D1 evaluation for other surface types
      auto surfaceD1 = [&](int, float u, float v,
                           float& Sx, float& Sy, float& Sz,
                           float& dSuX, float& dSuY, float& dSuZ,
                           float& dSvX, float& dSvY, float& dSvZ) {
        gp_Pnt S;
        gp_Vec dSu, dSv;
        surface.D1(static_cast<Standard_Real>(u), static_cast<Standard_Real>(v), S, dSu, dSv);
        Sx = static_cast<float>(S.X());
        Sy = static_cast<float>(S.Y());
        Sz = static_cast<float>(S.Z());
        dSuX = static_cast<float>(dSu.X());
        dSuY = static_cast<float>(dSu.Y());
        dSuZ = static_cast<float>(dSu.Z());
        dSvX = static_cast<float>(dSv.X());
        dSvY = static_cast<float>(dSv.Y());
        dSvZ = static_cast<float>(dSv.Z());
      };

      NewtonIterateBatch4_NEON(batch, surfaceD1, config);
    }
  }

  for (int i = 0; i < batch.ValidCount; ++i)
  {
    const PendingCurvedHit& hit = hits[startIdx + i];
    BRepIntCurveSurface_HitResult& result = results(hit.ResultIdx);

    if (batch.Converged[i] && batch.OutT[i] >= 0.0f)
    {
      result.Point = gp_Pnt(batch.OutPntX[i], batch.OutPntY[i], batch.OutPntZ[i]);
      result.U = static_cast<Standard_Real>(batch.OutU[i]);
      result.V = static_cast<Standard_Real>(batch.OutV[i]);
      result.W = static_cast<Standard_Real>(batch.OutT[i]);
    }
    else
    {
      result.Point = hit.RayOrg.Translated(hit.HitT * gp_Vec(hit.RayDir));
      result.U = hit.InitU;
      result.V = hit.InitV;
      result.W = hit.HitT;
    }
  }
}

#endif // OCCT_RT_HAS_NEON

} // anonymous namespace

//=================================================================================================

void BRepIntCurveSurface_InterBVH::PerformBatch(
  const NCollection_Array1<gp_Lin>&                  theRays,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& theResults,
  const Standard_Integer                             theNumThreads)
{
  (void)theNumThreads; // Will be used for manual thread control if needed

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
  struct ThreadLocalStats
  {
    long long faceTests        = 0;
    long long newtonIters      = 0;
    long long newtonFailures   = 0;
    long long refinementTimeNs = 0;
    long long nodeTests        = 0; // BVH node tests (thread-local to avoid atomic contention)
  };

  // Thread-local surface adaptors type (for thread-safe surface evaluation)
  using ThreadLocalSurfaces = std::vector<Handle(Adaptor3d_Surface)>;

  // Lambda to process a single ray and refine the hit
  // Uses thread-local surface adaptors for thread safety
  // OPTIMIZED: Cached ray components, separate planar/curved paths, removed redundant operations
  auto processRayHit = [&](Standard_Integer     idx,
                           Standard_Integer     hitTriIdx,
                           Standard_Real        hitT,
                           Standard_Real        baryU,
                           Standard_Real        baryV,
                           const gp_Lin&        aRay,
                           ThreadLocalStats&    stats,
                           ThreadLocalSurfaces& localSurfaces) {
    if (hitTriIdx < 0 || hitTriIdx >= static_cast<Standard_Integer>(myTriangleInfo.size()))
      return;

    Standard_Integer hitFaceIdx = myTriangleInfo[hitTriIdx].FaceIndex;
    if (hitFaceIdx < 0 || hitFaceIdx >= static_cast<Standard_Integer>(mySurfaceAdaptors.size()))
      return;

    if (hitT < 0.0)
      return;  // Early exit for invalid hits

    stats.faceTests++;

    // OPTIMIZATION: Cache ray components once (avoid repeated gp_Vec construction)
    const gp_Pnt& rayOrg = aRay.Location();
    const gp_Dir& rayDir = aRay.Direction();

    // Compute UV from barycentric coordinates
    const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[hitTriIdx];
    Standard_Real baryW = 1.0 - baryU - baryV;
    Standard_Real initU = baryW * triInfo.UV0.X() + baryU * triInfo.UV1.X() + baryV * triInfo.UV2.X();
    Standard_Real initV = baryW * triInfo.UV0.Y() + baryU * triInfo.UV1.Y() + baryV * triInfo.UV2.Y();

    // Get thread-local surface adaptor
    const Adaptor3d_Surface& aSurface = *localSurfaces[hitFaceIdx];

    // OPTIMIZATION: Check planarity early for separate code paths
    Standard_Boolean isPlanar = mySkipNewtonForPlanar
                                && hitFaceIdx < static_cast<Standard_Integer>(myIsPlanarFace.size())
                                && myIsPlanarFace[hitFaceIdx];

    BRepIntCurveSurface_HitResult& aResult = theResults(idx);
    aResult.IsValid    = Standard_True;
    aResult.FaceIndex  = hitFaceIdx + 1;
    aResult.Transition = IntCurveSurface_In;
    aResult.State      = TopAbs_IN;

    // OPTIMIZATION: Completely separate paths for planar vs curved faces
    if (isPlanar)
    {
      // PLANAR PATH: No Newton, no timing overhead, direct assignment
      aResult.Point = rayOrg.Translated(hitT * gp_Vec(rayDir));
      aResult.U     = initU;
      aResult.V     = initV;
      aResult.W     = hitT;

      // For planar faces, normal is constant - use D1 (cheaper than D2)
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv;
      aSurface.D1(initU, initV, normPnt, dSdu, dSdv);
      gp_Vec normalVec = dSdu.Crossed(dSdv);
      Standard_Real normalMag = normalVec.Magnitude();

      if (normalMag > 1e-10)
      {
        normalVec.Normalize();
        // OPTIMIZATION: Use cached orientation instead of FindKey lookup
        if (myFaceReversed[hitFaceIdx])
          normalVec.Reverse();
        aResult.Normal = gp_Dir(normalVec);
      }
      else
      {
        aResult.Normal = gp_Dir(0, 0, 1);
      }
      // Curvatures are 0 for planar faces (already default)
    }
    else
    {
      // CURVED PATH: Newton refinement with timing
      auto t0 = std::chrono::high_resolution_clock::now();

      Standard_Real    finalU = initU;
      Standard_Real    finalV = initV;
      Standard_Real    finalT = hitT;
      gp_Pnt           finalPnt;
      Standard_Integer iterCount = 0;

      NewtonResult newtonResult = RefineIntersectionNewton(aSurface,
                                                           rayOrg,
                                                           rayDir,
                                                           finalU,
                                                           finalV,
                                                           finalT,
                                                           finalPnt,
                                                           iterCount,
                                                           myNewtonTolerance,
                                                           myNewtonMaxIter);

      auto t1 = std::chrono::high_resolution_clock::now();
      stats.refinementTimeNs += std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
      stats.newtonIters += iterCount;

      if (newtonResult == NewtonResult::Converged && finalT >= 0.0)
      {
        aResult.Point = finalPnt;
        aResult.U     = finalU;
        aResult.V     = finalV;
        aResult.W     = finalT;
      }
      else
      {
        // Newton failed - use triangle intersection
        stats.newtonFailures++;
        aResult.Point = rayOrg.Translated(hitT * gp_Vec(rayDir));
        aResult.U     = initU;
        aResult.V     = initV;
        aResult.W     = hitT;
      }

      // Compute surface normal (and optionally curvatures)
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv;
      gp_Vec normalVec;

      // OPTIMIZATION: Use cached face orientation instead of FindKey lookup
      Standard_Boolean faceReversed = myFaceReversed[hitFaceIdx];

      if (myComputeCurvature)
      {
        // Full D2 evaluation for curvatures
        gp_Vec d2Sdu2, d2Sdv2, d2Sduv;
        aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          if (faceReversed)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);

          // Compute curvatures from first and second fundamental forms
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
            aResult.MeanCurvature     = (E * N - 2.0 * F * M + G * L) / (2.0 * denom);
            Standard_Real disc =
              aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
            Standard_Real sqrtDisc = std::sqrt(std::max(0.0, disc));
            aResult.MinCurvature   = aResult.MeanCurvature - sqrtDisc;
            aResult.MaxCurvature   = aResult.MeanCurvature + sqrtDisc;
          }
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
      }
      else
      {
        // D1 only - just compute normal (much cheaper than D2)
        aSurface.D1(aResult.U, aResult.V, normPnt, dSdu, dSdv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          if (faceReversed)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
      }
    }
  };

  // Helper to create thread-local surface copies
  auto createLocalSurfaces = [&]() -> ThreadLocalSurfaces {
    ThreadLocalSurfaces surfaces(mySurfaceAdaptors.size());
    for (size_t i = 0; i < mySurfaceAdaptors.size(); ++i)
    {
      surfaces[i] = mySurfaceAdaptors[i]->ShallowCopy();
    }
    return surfaces;
  };

  // Choose backend and parallelization strategy
  BRepIntCurveSurface_BVHBackend effectiveBackend = myBackend;

#ifdef OCCT_USE_EMBREE
  // Embree is available - check if we have a scene
  if (!myEmbreeScene
      && (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_Scalar
          || effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4
          || effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8))
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

  //=============================================================================
  // SIMD Newton Two-Phase Path (when enabled)
  //=============================================================================
  if (myUseSIMDNewton)
  {
    std::cout << "  [SIMD Newton] Two-phase processing enabled" << std::endl;
    auto simdStartTime = std::chrono::high_resolution_clock::now();

    // Structure to hold raw hit data from BVH traversal
    struct RawHit
    {
      Standard_Integer ResultIdx;
      Standard_Integer TriIdx;
      Standard_Real    HitT;
      Standard_Real    BaryU, BaryV;
      gp_Pnt           RayOrg;
      gp_Dir           RayDir;
    };

    std::vector<RawHit> rawHits;
    rawHits.reserve(nRays);

    // PHASE 1: BVH traversal - collect all hits
    // Use Embree8 when available for best performance
    auto phase1Start = std::chrono::high_resolution_clock::now();

    bool useEmbree = (effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8 ||
                      effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4 ||
                      effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_Scalar) &&
                     myEmbreeScene != nullptr;

    if (useEmbree && effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8)
    {
      // Embree SIMD8 path - process 8 rays at a time
#ifdef _OPENMP
      if (myUseOpenMP)
      {
        std::vector<std::vector<RawHit>> threadHits(omp_get_max_threads());
        #pragma omp parallel
        {
          int tid = omp_get_thread_num();
          threadHits[tid].reserve(nRays / omp_get_num_threads() + 100);

          #pragma omp for schedule(dynamic, 64)
          for (Standard_Integer i = 0; i < nRays; i += 8)
          {
            Standard_Integer batchSize = std::min(8, nRays - i);

            gp_Lin rays[8];
            for (int j = 0; j < 8; ++j)
            {
              if (j < batchSize)
                rays[j] = theRays(theRays.Lower() + i + j);
              else
                rays[j] = rays[0]; // Pad with first ray
            }

            Standard_Integer triIdx[8];
            Standard_Real hitT[8], baryU[8], baryV[8];
            IntersectEmbree8(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

            for (int j = 0; j < batchSize; ++j)
            {
              if (triIdx[j] >= 0)
              {
                RawHit hit;
                hit.ResultIdx = theRays.Lower() + i + j;
                hit.TriIdx = triIdx[j];
                hit.HitT = hitT[j];
                hit.BaryU = baryU[j];
                hit.BaryV = baryV[j];
                hit.RayOrg = rays[j].Location();
                hit.RayDir = rays[j].Direction();
                threadHits[tid].push_back(hit);
              }
            }
          }
        }
        // Merge thread-local hits
        for (const auto& th : threadHits)
          rawHits.insert(rawHits.end(), th.begin(), th.end());
      }
      else
#endif
      {
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
            if (triIdx[j] >= 0)
            {
              RawHit hit;
              hit.ResultIdx = theRays.Lower() + i + j;
              hit.TriIdx = triIdx[j];
              hit.HitT = hitT[j];
              hit.BaryU = baryU[j];
              hit.BaryV = baryV[j];
              hit.RayOrg = rays[j].Location();
              hit.RayDir = rays[j].Direction();
              rawHits.push_back(hit);
            }
          }
        }
      }
    }
    else if (useEmbree && effectiveBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4)
    {
      // Embree SIMD4 path - process 4 rays at a time
#ifdef _OPENMP
      if (myUseOpenMP)
      {
        std::vector<std::vector<RawHit>> threadHits(omp_get_max_threads());
        #pragma omp parallel
        {
          int tid = omp_get_thread_num();
          threadHits[tid].reserve(nRays / omp_get_num_threads() + 100);

          #pragma omp for schedule(dynamic, 64)
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
              if (triIdx[j] >= 0)
              {
                RawHit hit;
                hit.ResultIdx = theRays.Lower() + i + j;
                hit.TriIdx = triIdx[j];
                hit.HitT = hitT[j];
                hit.BaryU = baryU[j];
                hit.BaryV = baryV[j];
                hit.RayOrg = rays[j].Location();
                hit.RayDir = rays[j].Direction();
                threadHits[tid].push_back(hit);
              }
            }
          }
        }
        for (const auto& th : threadHits)
          rawHits.insert(rawHits.end(), th.begin(), th.end());
      }
      else
#endif
      {
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
            if (triIdx[j] >= 0)
            {
              RawHit hit;
              hit.ResultIdx = theRays.Lower() + i + j;
              hit.TriIdx = triIdx[j];
              hit.HitT = hitT[j];
              hit.BaryU = baryU[j];
              hit.BaryV = baryV[j];
              hit.RayOrg = rays[j].Location();
              hit.RayDir = rays[j].Direction();
              rawHits.push_back(hit);
            }
          }
        }
      }
    }
    else
    {
      // OCCT BVH fallback path
#ifdef _OPENMP
      if (myUseOpenMP)
      {
        std::vector<std::vector<RawHit>> threadHits(omp_get_max_threads());
        #pragma omp parallel
        {
          int tid = omp_get_thread_num();
          threadHits[tid].reserve(nRays / omp_get_num_threads() + 100);

          #pragma omp for schedule(dynamic, 64)
          for (Standard_Integer i = 0; i < nRays; ++i)
          {
            Standard_Integer idx  = theRays.Lower() + i;
            const gp_Lin&    aRay = theRays(idx);

            BRepIntCurveSurface_TriangleTraverser aTriTraverser;
            aTriTraverser.SetTriBVH(myTriBVH.get());
            aTriTraverser.SetTriangleInfo(&myTriangleInfo);
            aTriTraverser.SetRay(aRay, 0.0, RealLast());
            aTriTraverser.Select();

            Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
            if (hitTriIdx >= 0)
            {
              RawHit hit;
              hit.ResultIdx = idx;
              hit.TriIdx    = hitTriIdx;
              hit.HitT      = aTriTraverser.GetHitT();
              aTriTraverser.GetHitBarycentric(hit.BaryU, hit.BaryV);
              hit.RayOrg = aRay.Location();
              hit.RayDir = aRay.Direction();
              threadHits[tid].push_back(hit);
            }
          }
        }
        // Merge thread-local hits
        for (const auto& th : threadHits)
          rawHits.insert(rawHits.end(), th.begin(), th.end());
      }
      else
#endif
      {
        for (Standard_Integer i = 0; i < nRays; ++i)
        {
          Standard_Integer idx  = theRays.Lower() + i;
          const gp_Lin&    aRay = theRays(idx);

          BRepIntCurveSurface_TriangleTraverser aTriTraverser;
          aTriTraverser.SetTriBVH(myTriBVH.get());
          aTriTraverser.SetTriangleInfo(&myTriangleInfo);
          aTriTraverser.SetRay(aRay, 0.0, RealLast());
          aTriTraverser.Select();

          Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
          if (hitTriIdx >= 0)
          {
            RawHit hit;
            hit.ResultIdx = idx;
            hit.TriIdx    = hitTriIdx;
            hit.HitT      = aTriTraverser.GetHitT();
            aTriTraverser.GetHitBarycentric(hit.BaryU, hit.BaryV);
            hit.RayOrg = aRay.Location();
            hit.RayDir = aRay.Direction();
            rawHits.push_back(hit);
          }
        }
      }
    }

    auto phase1End = std::chrono::high_resolution_clock::now();
    double phase1Ms = std::chrono::duration<double, std::milli>(phase1End - phase1Start).count();
    std::cout << "  [SIMD Newton] Phase 1 (BVH traversal): " << rawHits.size() << " hits in "
              << std::fixed << std::setprecision(2) << phase1Ms << " ms" << std::endl;

    // PHASE 2: Process hits - separate planar and curved
    auto phase2Start = std::chrono::high_resolution_clock::now();

    // Group curved hits by face for batching
    std::unordered_map<Standard_Integer, std::vector<PendingCurvedHit>> curvedHitsByFace;
    Standard_Integer planarCount = 0;
    Standard_Integer curvedCount = 0;

    ThreadLocalSurfaces localSurfaces = createLocalSurfaces();

    for (const RawHit& raw : rawHits)
    {
      if (raw.TriIdx < 0 || raw.TriIdx >= static_cast<Standard_Integer>(myTriangleInfo.size()))
        continue;

      Standard_Integer hitFaceIdx = myTriangleInfo[raw.TriIdx].FaceIndex;
      if (hitFaceIdx < 0 || hitFaceIdx >= static_cast<Standard_Integer>(mySurfaceAdaptors.size()))
        continue;

      // Compute initial UV from barycentric
      const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[raw.TriIdx];
      Standard_Real baryW = 1.0 - raw.BaryU - raw.BaryV;
      Standard_Real initU = baryW * triInfo.UV0.X() + raw.BaryU * triInfo.UV1.X() + raw.BaryV * triInfo.UV2.X();
      Standard_Real initV = baryW * triInfo.UV0.Y() + raw.BaryU * triInfo.UV1.Y() + raw.BaryV * triInfo.UV2.Y();

      const Adaptor3d_Surface& aSurface = *localSurfaces[hitFaceIdx];

      // Initialize result
      BRepIntCurveSurface_HitResult& aResult = theResults(raw.ResultIdx);
      aResult.IsValid    = Standard_True;
      aResult.FaceIndex  = hitFaceIdx + 1;
      aResult.Transition = IntCurveSurface_In;
      aResult.State      = TopAbs_IN;

      Standard_Boolean isPlanar = mySkipNewtonForPlanar
                                  && hitFaceIdx < static_cast<Standard_Integer>(myIsPlanarFace.size())
                                  && myIsPlanarFace[hitFaceIdx];

      if (isPlanar)
      {
        // PLANAR: Process directly (no Newton needed)
        planarCount++;
        aResult.Point = raw.RayOrg.Translated(raw.HitT * gp_Vec(raw.RayDir));
        aResult.U     = initU;
        aResult.V     = initV;
        aResult.W     = raw.HitT;

        // Compute normal
        gp_Pnt normPnt;
        gp_Vec dSdu, dSdv;
        aSurface.D1(initU, initV, normPnt, dSdu, dSdv);
        gp_Vec normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();
        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          if (myFaceReversed[hitFaceIdx])
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
      }
      else
      {
        // CURVED: Collect for batched SIMD Newton
        curvedCount++;
        PendingCurvedHit pending;
        pending.ResultIdx = raw.ResultIdx;
        pending.FaceIdx   = hitFaceIdx;
        pending.TriIdx    = raw.TriIdx;
        pending.HitT      = raw.HitT;
        pending.BaryU     = raw.BaryU;
        pending.BaryV     = raw.BaryV;
        pending.InitU     = initU;
        pending.InitV     = initV;
        pending.RayOrg    = raw.RayOrg;
        pending.RayDir    = raw.RayDir;
        // Copy knot span indices for B-spline optimization
        pending.USpanIndex = triInfo.USpanIndex;
        pending.VSpanIndex = triInfo.VSpanIndex;
        curvedHitsByFace[hitFaceIdx].push_back(pending);
      }
    }

    std::cout << "  [SIMD Newton] Planar hits: " << planarCount << ", Curved hits: " << curvedCount << std::endl;

    // Convert map to vector for OpenMP parallel iteration
    std::vector<std::pair<Standard_Integer, std::vector<PendingCurvedHit>*>> faceHitPairs;
    faceHitPairs.reserve(curvedHitsByFace.size());
    for (auto& [faceIdx, hits] : curvedHitsByFace)
    {
      faceHitPairs.push_back({faceIdx, &hits});
    }

    // Process curved hits in batches per face - WITH OPENMP
    std::atomic<Standard_Integer> simdBatches{0};
    std::atomic<Standard_Integer> totalNewtonIters{0};

    auto processFaceHits = [&](Standard_Integer faceIdx, std::vector<PendingCurvedHit>& hits,
                               const Adaptor3d_Surface& aSurface) {
      Standard_Real uMin = aSurface.FirstUParameter();
      Standard_Real uMax = aSurface.LastUParameter();
      Standard_Real vMin = aSurface.FirstVParameter();
      Standard_Real vMax = aSurface.LastVParameter();

      size_t numHits = hits.size();
      Standard_Integer localBatches = 0;
      Standard_Integer localIters = 0;

#ifdef OCCT_RT_HAS_AVX
      for (size_t i = 0; i < numHits; i += 8)
      {
        size_t batchSize = std::min(size_t(8), numHits - i);
        ProcessNewtonBatch8_AVX(hits, i, batchSize, aSurface,
                                uMin, uMax, vMin, vMax,
                                myNewtonTolerance, myNewtonMaxIter,
                                theResults);
        localBatches++;
        localIters += static_cast<Standard_Integer>(batchSize) * 3;
      }
#elif defined(OCCT_RT_HAS_SSE)
      for (size_t i = 0; i < numHits; i += 4)
      {
        size_t batchSize = std::min(size_t(4), numHits - i);
        ProcessNewtonBatch4_SSE(hits, i, batchSize, aSurface,
                                uMin, uMax, vMin, vMax,
                                myNewtonTolerance, myNewtonMaxIter,
                                theResults);
        localBatches++;
        localIters += static_cast<Standard_Integer>(batchSize) * 3;
      }
#elif defined(OCCT_RT_HAS_NEON)
      // Pre-compute B-spline cache once for this face (if B-spline surface)
      BSplineCacheData bsplineCacheData;
      BSplineCacheData* bsplineCachePtr = nullptr;
      if (aSurface.GetType() == GeomAbs_BSplineSurface)
      {
        Handle(Geom_BSplineSurface) bspline = aSurface.BSpline();
        if (!bspline.IsNull())
        {
          bsplineCacheData.Init(bspline);
          bsplineCachePtr = &bsplineCacheData;
        }
      }

      // Check if this surface uses analytic intersection (skip Newton counter)
      GeomAbs_SurfaceType surfTypeForAnalytic = aSurface.GetType();
      bool usesAnalytic = (surfTypeForAnalytic == GeomAbs_Plane || surfTypeForAnalytic == GeomAbs_Sphere ||
                           surfTypeForAnalytic == GeomAbs_Cylinder || surfTypeForAnalytic == GeomAbs_Cone ||
                           surfTypeForAnalytic == GeomAbs_Torus);

      for (size_t i = 0; i < numHits; i += 4)
      {
        size_t batchSize = std::min(size_t(4), numHits - i);
        ProcessNewtonBatch4_NEON(hits, i, batchSize, aSurface,
                                 uMin, uMax, vMin, vMax,
                                 myNewtonTolerance, myNewtonMaxIter,
                                 theResults, bsplineCachePtr);
        localBatches++;
        // Only count Newton iterations for surfaces that actually use Newton
        if (!usesAnalytic)
          localIters += static_cast<Standard_Integer>(batchSize) * 3;
      }
#else
      for (const PendingCurvedHit& hit : hits)
      {
        BRepIntCurveSurface_HitResult& aResult = theResults(hit.ResultIdx);
        Standard_Real finalU = hit.InitU;
        Standard_Real finalV = hit.InitV;
        Standard_Real finalT = hit.HitT;
        gp_Pnt finalPnt;
        Standard_Integer iterCount = 0;

        NewtonResult newtonResult = RefineIntersectionNewton(aSurface,
                                                             hit.RayOrg, hit.RayDir,
                                                             finalU, finalV, finalT, finalPnt,
                                                             iterCount,
                                                             myNewtonTolerance, myNewtonMaxIter);
        localIters += iterCount;

        if (newtonResult == NewtonResult::Converged && finalT >= 0.0)
        {
          aResult.Point = finalPnt;
          aResult.U     = finalU;
          aResult.V     = finalV;
          aResult.W     = finalT;
        }
        else
        {
          aResult.Point = hit.RayOrg.Translated(hit.HitT * gp_Vec(hit.RayDir));
          aResult.U     = hit.InitU;
          aResult.V     = hit.InitV;
          aResult.W     = hit.HitT;
        }
      }
#endif

      simdBatches += localBatches;
      totalNewtonIters += localIters;

      // Compute normals - SIMD batched when possible
      GeomAbs_SurfaceType surfType = aSurface.GetType();
      bool useSIMDNormal = SupportsSIMDSurface(surfType);
      SIMDSurfaceParams surfParams;

      if (useSIMDNormal)
      {
        switch (surfType)
        {
          case GeomAbs_Torus:
          {
            gp_Torus torus = aSurface.Torus();
            surfParams.Init(torus.Position(), GeomAbs_Torus,
                            0.0, torus.MajorRadius(), torus.MinorRadius());
            break;
          }
          case GeomAbs_Cylinder:
          {
            gp_Cylinder cyl = aSurface.Cylinder();
            surfParams.Init(cyl.Position(), GeomAbs_Cylinder, cyl.Radius());
            break;
          }
          case GeomAbs_Sphere:
          {
            gp_Sphere sph = aSurface.Sphere();
            surfParams.Init(sph.Position(), GeomAbs_Sphere, sph.Radius());
            break;
          }
          default:
            useSIMDNormal = false;
            break;
        }
      }

      Standard_Boolean faceReversed = myFaceReversed[faceIdx];
      size_t numHitsForNormal = hits.size();

      if (useSIMDNormal)
      {
        for (size_t i = 0; i < numHitsForNormal; i += 4)
        {
          size_t batchSize = std::min(size_t(4), numHitsForNormal - i);

          alignas(16) float uBatch[4], vBatch[4];
          for (size_t j = 0; j < batchSize; ++j)
          {
            const PendingCurvedHit& hit = hits[i + j];
            BRepIntCurveSurface_HitResult& aResult = theResults(hit.ResultIdx);
            uBatch[j] = static_cast<float>(aResult.U);
            vBatch[j] = static_cast<float>(aResult.V);
          }
          for (size_t j = batchSize; j < 4; ++j)
          {
            uBatch[j] = uBatch[0];
            vBatch[j] = vBatch[0];
          }

          if (myComputeCurvature)
          {
            alignas(16) float Px[4], Py[4], Pz[4];
            alignas(16) float dSuX[4], dSuY[4], dSuZ[4];
            alignas(16) float dSvX[4], dSvY[4], dSvZ[4];
            alignas(16) float d2SuuX[4], d2SuuY[4], d2SuuZ[4];
            alignas(16) float d2SvvX[4], d2SvvY[4], d2SvvZ[4];
            alignas(16) float d2SuvX[4], d2SuvY[4], d2SuvZ[4];

            SurfaceD2_4(surfParams, uBatch, vBatch,
                        Px, Py, Pz,
                        dSuX, dSuY, dSuZ,
                        dSvX, dSvY, dSvZ,
                        d2SuuX, d2SuuY, d2SuuZ,
                        d2SvvX, d2SvvY, d2SvvZ,
                        d2SuvX, d2SuvY, d2SuvZ);

            for (size_t j = 0; j < batchSize; ++j)
            {
              const PendingCurvedHit& hit = hits[i + j];
              BRepIntCurveSurface_HitResult& aResult = theResults(hit.ResultIdx);

              float nx = dSuY[j] * dSvZ[j] - dSuZ[j] * dSvY[j];
              float ny = dSuZ[j] * dSvX[j] - dSuX[j] * dSvZ[j];
              float nz = dSuX[j] * dSvY[j] - dSuY[j] * dSvX[j];
              float normalMag = std::sqrt(nx*nx + ny*ny + nz*nz);

              if (normalMag > 1e-10f)
              {
                nx /= normalMag; ny /= normalMag; nz /= normalMag;
                if (faceReversed) { nx = -nx; ny = -ny; nz = -nz; }
                aResult.Normal = gp_Dir(nx, ny, nz);

                float E = dSuX[j]*dSuX[j] + dSuY[j]*dSuY[j] + dSuZ[j]*dSuZ[j];
                float F = dSuX[j]*dSvX[j] + dSuY[j]*dSvY[j] + dSuZ[j]*dSvZ[j];
                float G = dSvX[j]*dSvX[j] + dSvY[j]*dSvY[j] + dSvZ[j]*dSvZ[j];
                float L = d2SuuX[j]*nx + d2SuuY[j]*ny + d2SuuZ[j]*nz;
                float M = d2SuvX[j]*nx + d2SuvY[j]*ny + d2SuvZ[j]*nz;
                float N = d2SvvX[j]*nx + d2SvvY[j]*ny + d2SvvZ[j]*nz;

                float denom = E * G - F * F;
                if (std::abs(denom) > 1e-20f)
                {
                  aResult.GaussianCurvature = (L * N - M * M) / denom;
                  aResult.MeanCurvature = (E * N - 2.0f * F * M + G * L) / (2.0f * denom);
                  float disc = aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
                  float sqrtDisc = std::sqrt(std::max(0.0f, disc));
                  aResult.MinCurvature = aResult.MeanCurvature - sqrtDisc;
                  aResult.MaxCurvature = aResult.MeanCurvature + sqrtDisc;
                }
              }
              else
              {
                aResult.Normal = gp_Dir(0, 0, 1);
              }
            }
          }
          else
          {
            alignas(16) float Px[4], Py[4], Pz[4];
            alignas(16) float dSuX[4], dSuY[4], dSuZ[4];
            alignas(16) float dSvX[4], dSvY[4], dSvZ[4];

            SurfaceD1_4(surfParams, uBatch, vBatch,
                        Px, Py, Pz,
                        dSuX, dSuY, dSuZ,
                        dSvX, dSvY, dSvZ);

            for (size_t j = 0; j < batchSize; ++j)
            {
              const PendingCurvedHit& hit = hits[i + j];
              BRepIntCurveSurface_HitResult& aResult = theResults(hit.ResultIdx);

              float nx = dSuY[j] * dSvZ[j] - dSuZ[j] * dSvY[j];
              float ny = dSuZ[j] * dSvX[j] - dSuX[j] * dSvZ[j];
              float nz = dSuX[j] * dSvY[j] - dSuY[j] * dSvX[j];
              float normalMag = std::sqrt(nx*nx + ny*ny + nz*nz);

              if (normalMag > 1e-10f)
              {
                nx /= normalMag; ny /= normalMag; nz /= normalMag;
                if (faceReversed) { nx = -nx; ny = -ny; nz = -nz; }
                aResult.Normal = gp_Dir(nx, ny, nz);
              }
              else
              {
                aResult.Normal = gp_Dir(0, 0, 1);
              }
            }
          }
        }
      }
      else
      {
        for (const PendingCurvedHit& hit : hits)
        {
          BRepIntCurveSurface_HitResult& aResult = theResults(hit.ResultIdx);
          gp_Pnt normPnt;
          gp_Vec dSdu, dSdv;

          if (myComputeCurvature)
          {
            gp_Vec d2Sdu2, d2Sdv2, d2Sduv;
            aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
            gp_Vec normalVec = dSdu.Crossed(dSdv);
            Standard_Real normalMag = normalVec.Magnitude();

            if (normalMag > 1e-10)
            {
              normalVec.Normalize();
              if (faceReversed)
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
          else
          {
            aSurface.D1(aResult.U, aResult.V, normPnt, dSdu, dSdv);
            gp_Vec normalVec = dSdu.Crossed(dSdv);
            Standard_Real normalMag = normalVec.Magnitude();
            if (normalMag > 1e-10)
            {
              normalVec.Normalize();
              if (faceReversed)
                normalVec.Reverse();
              aResult.Normal = gp_Dir(normalVec);
            }
            else
            {
              aResult.Normal = gp_Dir(0, 0, 1);
            }
          }
        }
      }
    };

    // Execute with or without OpenMP
#ifdef _OPENMP
    if (myUseOpenMP && faceHitPairs.size() > 1)
    {
      // Parallel processing across faces
      #pragma omp parallel
      {
        // Each thread needs its own surface copies
        ThreadLocalSurfaces threadSurfaces = createLocalSurfaces();

        #pragma omp for schedule(dynamic)
        for (size_t fi = 0; fi < faceHitPairs.size(); ++fi)
        {
          Standard_Integer faceIdx = faceHitPairs[fi].first;
          std::vector<PendingCurvedHit>& hits = *faceHitPairs[fi].second;
          const Adaptor3d_Surface& aSurface = *threadSurfaces[faceIdx];
          processFaceHits(faceIdx, hits, aSurface);
        }
      }
    }
    else
#endif
    {
      // Single-threaded processing
      for (auto& [faceIdx, hitsPtr] : faceHitPairs)
      {
        const Adaptor3d_Surface& aSurface = *localSurfaces[faceIdx];
        processFaceHits(faceIdx, *hitsPtr, aSurface);
      }
    }

    auto phase2End = std::chrono::high_resolution_clock::now();
    double phase2Ms = std::chrono::duration<double, std::milli>(phase2End - phase2Start).count();

    auto simdEndTime = std::chrono::high_resolution_clock::now();
    double simdTotalSec = std::chrono::duration<double>(simdEndTime - simdStartTime).count();

    std::cout << "  [SIMD Newton] Phase 2 (Newton + normals): " << std::fixed << std::setprecision(2)
              << phase2Ms << " ms, " << simdBatches << " SIMD batches" << std::endl;
    std::cout << "  [SIMD Newton] Total time: " << simdTotalSec << "s, "
              << std::setprecision(0) << (nRays / simdTotalSec) << " rays/sec" << std::endl;

    totalStats.faceTests = rawHits.size();
    totalStats.newtonIters = totalNewtonIters;
  }
  else
  //=============================================================================
  // Original Single-Ray Processing Path (when SIMD Newton disabled)
  //=============================================================================
  if (effectiveBackend == BRepIntCurveSurface_BVHBackend::OCCT_BVH)
  {
// OCCT BVH backend (no Embree dependency)
#ifdef _OPENMP
    if (myUseOpenMP)
    {
  #pragma omp parallel
      {
        ThreadLocalStats    localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces(); // Thread-local surface copies
  #pragma omp for schedule(dynamic, 64)
        for (Standard_Integer i = 0; i < nRays; ++i)
        {
          Standard_Integer idx  = theRays.Lower() + i;
          const gp_Lin&    aRay = theRays(idx);

          BRepIntCurveSurface_TriangleTraverser aTriTraverser;
          aTriTraverser.SetTriBVH(myTriBVH.get());
          aTriTraverser.SetTriangleInfo(&myTriangleInfo);
          aTriTraverser.SetRay(aRay, 0.0, RealLast());
          aTriTraverser.Select();

          // Accumulate thread-local node test count (no atomic overhead!)
          localStats.nodeTests += aTriTraverser.GetNodeTestCount();

          Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
          Standard_Real    hitT      = aTriTraverser.GetHitT();
          Standard_Real    baryU, baryV;
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
      ThreadLocalStats    localStats;
      ThreadLocalSurfaces localSurfaces =
        createLocalSurfaces(); // Single-threaded uses same pattern
      for (Standard_Integer i = 0; i < nRays; ++i)
      {
        Standard_Integer idx  = theRays.Lower() + i;
        const gp_Lin&    aRay = theRays(idx);

        BRepIntCurveSurface_TriangleTraverser aTriTraverser;
        aTriTraverser.SetTriBVH(myTriBVH.get());
        aTriTraverser.SetTriangleInfo(&myTriangleInfo);
        aTriTraverser.SetRay(aRay, 0.0, RealLast());
        aTriTraverser.Select();

        // Accumulate thread-local node test count
        localStats.nodeTests += aTriTraverser.GetNodeTestCount();

        Standard_Integer hitTriIdx = aTriTraverser.GetHitTriangleIndex();
        Standard_Real    hitT      = aTriTraverser.GetHitT();
        Standard_Real    baryU, baryV;
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
        ThreadLocalStats    localStats;
        ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
    #pragma omp for schedule(dynamic, 64)
        for (Standard_Integer i = 0; i < nRays; ++i)
        {
          Standard_Integer idx  = theRays.Lower() + i;
          const gp_Lin&    aRay = theRays(idx);

          Standard_Integer hitTriIdx;
          Standard_Real    hitT, baryU, baryV;
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
      ThreadLocalStats    localStats;
      ThreadLocalSurfaces localSurfaces = createLocalSurfaces();
      for (Standard_Integer i = 0; i < nRays; ++i)
      {
        Standard_Integer idx  = theRays.Lower() + i;
        const gp_Lin&    aRay = theRays(idx);

        Standard_Integer hitTriIdx;
        Standard_Real    hitT, baryU, baryV;
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
        ThreadLocalStats    localStats;
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
              rays[j] = rays[0]; // Duplicate first ray for padding
          }

          Standard_Integer triIdx[4];
          Standard_Real    hitT[4], baryU[4], baryV[4];
          IntersectEmbree4(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

          // Process results
          for (int j = 0; j < batchSize; ++j)
          {
            Standard_Integer idx = theRays.Lower() + i + j;
            processRayHit(idx,
                          triIdx[j],
                          hitT[j],
                          baryU[j],
                          baryV[j],
                          rays[j],
                          localStats,
                          localSurfaces);
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
      ThreadLocalStats    localStats;
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
        Standard_Real    hitT[4], baryU[4], baryV[4];
        IntersectEmbree4(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

        for (int j = 0; j < batchSize; ++j)
        {
          Standard_Integer idx = theRays.Lower() + i + j;
          processRayHit(idx,
                        triIdx[j],
                        hitT[j],
                        baryU[j],
                        baryV[j],
                        rays[j],
                        localStats,
                        localSurfaces);
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
        ThreadLocalStats    localStats;
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
          Standard_Real    hitT[8], baryU[8], baryV[8];
          IntersectEmbree8(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

          for (int j = 0; j < batchSize; ++j)
          {
            Standard_Integer idx = theRays.Lower() + i + j;
            processRayHit(idx,
                          triIdx[j],
                          hitT[j],
                          baryU[j],
                          baryV[j],
                          rays[j],
                          localStats,
                          localSurfaces);
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
      ThreadLocalStats    localStats;
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
        Standard_Real    hitT[8], baryU[8], baryV[8];
        IntersectEmbree8(myEmbreeScene, rays, triIdx, hitT, baryU, baryV);

        for (int j = 0; j < batchSize; ++j)
        {
          Standard_Integer idx = theRays.Lower() + i + j;
          processRayHit(idx,
                        triIdx[j],
                        hitT[j],
                        baryU[j],
                        baryV[j],
                        rays[j],
                        localStats,
                        localSurfaces);
        }
      }
      totalStats = localStats;
    }
  }
#endif

  // Print debug stats (using thread-local accumulated stats, no atomic contention)
  auto   endTime  = std::chrono::high_resolution_clock::now();
  double totalSec = std::chrono::duration<double>(endTime - startTime).count();

  std::cout << "  Total time: " << std::fixed << std::setprecision(2) << totalSec << "s, "
            << std::setprecision(0) << (nRays / totalSec) << " rays/sec" << std::endl;
  std::cout << "  [DEBUG] Triangle BVH node tests: " << totalStats.nodeTests << " ("
            << (double)totalStats.nodeTests / nRays << " per ray)" << std::endl;
  std::cout << "  [DEBUG] Face intersection tests: " << totalStats.faceTests << " ("
            << (double)totalStats.faceTests / nRays << " per ray)" << std::endl;
  if (totalStats.faceTests > 0)
  {
    std::cout << "  [DEBUG] Surface refinement time: " << totalStats.refinementTimeNs / 1000000.0
              << " ms (" << (double)totalStats.refinementTimeNs / totalStats.faceTests / 1000.0
              << " us per call)" << std::endl;
  }
  if (totalStats.newtonIters > 0)
  {
    std::cout << "  [DEBUG] Newton iterations: " << totalStats.newtonIters << " ("
              << (double)totalStats.newtonIters / totalStats.faceTests << " per hit)" << std::endl;
  }
  if (totalStats.newtonFailures > 0)
  {
    std::cout << "  [DEBUG] Newton refinement failures: " << totalStats.newtonFailures
              << " (used triangle fallback)" << std::endl;
  }
}

//=================================================================================================

void BRepIntCurveSurface_InterBVH::PerformBatchCount(
  const NCollection_Array1<gp_Lin>&     theRays,
  NCollection_Array1<Standard_Integer>& theHitCounts,
  const Standard_Integer                theNumThreads)
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
  auto             startTime    = std::chrono::high_resolution_clock::now();

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
    Standard_Integer idx  = theRays.Lower() + i;
    const gp_Lin&    aRay = theRays(idx);

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
      auto   now     = std::chrono::high_resolution_clock::now();
      double elapsed = std::chrono::duration<double>(now - startTime).count();
      std::cout << "  Progress: " << currentProgress << "% (" << std::fixed << std::setprecision(1)
                << elapsed << "s)" << std::endl;
      lastProgress = currentProgress;
    }
  }

  // Print debug stats
  std::cout << "  [DEBUG] Triangle BVH node tests: " << totalNodeTests << " (" << std::fixed
            << std::setprecision(1) << (double)totalNodeTests / nRays << " per ray)" << std::endl;
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

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::GaussianCurvature(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::GaussianCurvature - index out of range");
  return myResults[theIndex - 1].GaussianCurvature;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::MeanCurvature(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::MeanCurvature - index out of range");
  return myResults[theIndex - 1].MeanCurvature;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::MinCurvature(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::MinCurvature - index out of range");
  return myResults[theIndex - 1].MinCurvature;
}

//=================================================================================================

Standard_Real BRepIntCurveSurface_InterBVH::MaxCurvature(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::MaxCurvature - index out of range");
  return myResults[theIndex - 1].MaxCurvature;
}

//=================================================================================================

Standard_Integer BRepIntCurveSurface_InterBVH::FaceIndex(const Standard_Integer theIndex) const
{
  if (theIndex < 1 || theIndex > myNbPnt)
    throw Standard_OutOfRange("BRepIntCurveSurface_InterBVH::FaceIndex - index out of range");
  return myResults[theIndex - 1].FaceIndex;
}

//=================================================================================================
// Packet BVH Traverser - processes a packet of rays together for better cache efficiency
// Optimized with SIMD ray-box and ray-triangle tests, and frustum culling
//=================================================================================================

namespace
{
//! Packet BVH traverser that processes N rays together with SIMD acceleration
template <int N>
class BRepIntCurveSurface_PacketTraverser
{
public:
  BRepIntCurveSurface_PacketTraverser()
      : myTriBVH(nullptr),
        myTriangleInfo(nullptr),
        myNodeTestCount(0),
        myTriangleTestCount(0),
        myFrustumCulled(0),
        myUseSIMD(true)
  {
    // Pre-allocate stack
    myStack.reserve(64);
  }

  void SetTriBVH(BRepIntCurveSurface_TriBVH* theBVH) { myTriBVH = theBVH; }

  void SetTriangleInfo(const std::vector<BRepIntCurveSurface_TriangleInfo>* theInfo)
  {
    myTriangleInfo = theInfo;
  }

  void SetUseSIMD(bool theUseSIMD) { myUseSIMD = theUseSIMD; }

  //! Traverse the BVH with a ray packet (SIMD-accelerated)
  void TraversePacket(BRepIntCurveSurface_RayPacket<N>&       thePacket,
                      BRepIntCurveSurface_PacketHitResult<N>& theResult)
  {
    myNodeTestCount     = 0;
    myTriangleTestCount = 0;
    myFrustumCulled     = 0;

    if (myTriBVH == nullptr || myTriBVH->BVH().IsNull())
      return;

    const opencascade::handle<BVH_Tree<Standard_Real, 3>>& aBVH = myTriBVH->BVH();

    // Prepare float arrays for SIMD (convert from double)
    PrepareFloatArrays(thePacket, theResult);

    // Stack-based traversal with active mask
    myStack.clear();
    myStack.push_back({0, thePacket.ActiveMask});

    while (!myStack.empty())
    {
      StackEntry entry = myStack.back();
      myStack.pop_back();

      if (entry.ActiveMask == 0)
        continue;

      Standard_Integer aNodeIdx = entry.NodeIdx;
      if (aNodeIdx < 0)
        continue;

      // Get node bounds
      const BVH_Vec3d& aMinPt = aBVH->MinPoint(aNodeIdx);
      const BVH_Vec3d& aMaxPt = aBVH->MaxPoint(aNodeIdx);

      ++myNodeTestCount;

      // Quick frustum culling: test packet frustum against box
      if (!FrustumBoxIntersect(thePacket, aMinPt, aMaxPt))
      {
        ++myFrustumCulled;
        continue;
      }

      // SIMD ray-box test for all rays in packet
      uint32_t nodeActiveMask = TestBoxSIMD(thePacket, aMinPt, aMaxPt, entry.ActiveMask);

      if (nodeActiveMask == 0)
        continue;

      if (aBVH->IsOuter(aNodeIdx))
      {
        // Leaf node - test triangles with SIMD
        Standard_Integer aFirstTriIdx = aBVH->BegPrimitive(aNodeIdx);
        Standard_Integer aLastTriIdx  = aBVH->EndPrimitive(aNodeIdx);

        for (Standard_Integer triIdx = aFirstTriIdx; triIdx <= aLastTriIdx; ++triIdx)
        {
          TestTriangleSIMD(triIdx, thePacket, theResult, nodeActiveMask);
        }
      }
      else
      {
        // Inner node - push children with ordered traversal based on packet direction
        Standard_Integer aLeftChild  = aBVH->Child<0>(aNodeIdx);
        Standard_Integer aRightChild = aBVH->Child<1>(aNodeIdx);

        // Order children based on dominant direction for front-to-back traversal
        const BVH_Vec3d& leftMin = aBVH->MinPoint(aLeftChild);
        const BVH_Vec3d& rightMin = aBVH->MinPoint(aRightChild);

        // Use packet's dominant direction to order traversal
        bool leftFirst = true;
        if (thePacket.DirSignX > 0)
          leftFirst = leftMin[0] < rightMin[0];
        else if (thePacket.DirSignX < 0)
          leftFirst = leftMin[0] > rightMin[0];

        if (leftFirst)
        {
          myStack.push_back({aRightChild, nodeActiveMask}); // Process second
          myStack.push_back({aLeftChild, nodeActiveMask});  // Process first
        }
        else
        {
          myStack.push_back({aLeftChild, nodeActiveMask});
          myStack.push_back({aRightChild, nodeActiveMask});
        }
      }
    }
  }

  Standard_Integer GetNodeTestCount() const { return myNodeTestCount; }
  Standard_Integer GetTriangleTestCount() const { return myTriangleTestCount; }
  Standard_Integer GetFrustumCulled() const { return myFrustumCulled; }

private:
  struct StackEntry
  {
    Standard_Integer NodeIdx;
    uint32_t         ActiveMask;
  };

  // Pre-converted float arrays for SIMD operations
  alignas(32) float myOrgXf[N], myOrgYf[N], myOrgZf[N];
  alignas(32) float myDirXf[N], myDirYf[N], myDirZf[N];
  alignas(32) float myInvDirXf[N], myInvDirYf[N], myInvDirZf[N];
  alignas(32) float myTMinf[N], myTMaxf[N];
  alignas(32) float myCurrentTf[N];

  //! Prepare float arrays from packet for SIMD
  void PrepareFloatArrays(const BRepIntCurveSurface_RayPacket<N>& packet,
                          const BRepIntCurveSurface_PacketHitResult<N>& result)
  {
    for (int i = 0; i < N; ++i)
    {
      myOrgXf[i] = static_cast<float>(packet.OrgX[i]);
      myOrgYf[i] = static_cast<float>(packet.OrgY[i]);
      myOrgZf[i] = static_cast<float>(packet.OrgZ[i]);
      myDirXf[i] = static_cast<float>(packet.DirX[i]);
      myDirYf[i] = static_cast<float>(packet.DirY[i]);
      myDirZf[i] = static_cast<float>(packet.DirZ[i]);
      myInvDirXf[i] = static_cast<float>(packet.InvDirX[i]);
      myInvDirYf[i] = static_cast<float>(packet.InvDirY[i]);
      myInvDirZf[i] = static_cast<float>(packet.InvDirZ[i]);
      myTMinf[i] = static_cast<float>(packet.TMin[i]);
      myTMaxf[i] = static_cast<float>(result.HitT[i]);
      myCurrentTf[i] = static_cast<float>(result.HitT[i]);
    }
  }

  //! Quick frustum-box intersection test (before per-ray tests)
  bool FrustumBoxIntersect(const BRepIntCurveSurface_RayPacket<N>& packet,
                           const BVH_Vec3d& boxMin,
                           const BVH_Vec3d& boxMax) const
  {
    // Conservative test: check if box is completely outside packet frustum
    // Note: packet frustum is very conservative, so this mainly helps with distant nodes
    return !(packet.FrustumMax[0] < boxMin[0] || packet.FrustumMin[0] > boxMax[0] ||
             packet.FrustumMax[1] < boxMin[1] || packet.FrustumMin[1] > boxMax[1] ||
             packet.FrustumMax[2] < boxMin[2] || packet.FrustumMin[2] > boxMax[2]);
  }

  //! SIMD ray-box test for all rays in packet
  uint32_t TestBoxSIMD(const BRepIntCurveSurface_RayPacket<N>& packet,
                       const BVH_Vec3d& boxMin,
                       const BVH_Vec3d& boxMax,
                       uint32_t activeMask)
  {
    // Update current T values from result (for early termination)
    float boxMinXf = static_cast<float>(boxMin[0]);
    float boxMinYf = static_cast<float>(boxMin[1]);
    float boxMinZf = static_cast<float>(boxMin[2]);
    float boxMaxXf = static_cast<float>(boxMax[0]);
    float boxMaxYf = static_cast<float>(boxMax[1]);
    float boxMaxZf = static_cast<float>(boxMax[2]);

#if defined(OCCT_RT_HAS_AVX) && N >= 8
    if (myUseSIMD && N >= 8)
    {
      // AVX: test 8 rays at once
      int hitMask = SIMD::RayBox8_AVX(myOrgXf, myOrgYf, myOrgZf,
                                      myInvDirXf, myInvDirYf, myInvDirZf,
                                      myTMinf, myCurrentTf,
                                      boxMinXf, boxMinYf, boxMinZf,
                                      boxMaxXf, boxMaxYf, boxMaxZf);
      return static_cast<uint32_t>(hitMask) & activeMask;
    }
#endif

#if defined(OCCT_RT_HAS_SSE) && N >= 4
    if (myUseSIMD && N >= 4)
    {
      // SSE: test 4 rays at once (do 2 passes for 8-ray packets)
      uint32_t result = 0;

      // First 4 rays
      int hitMask1 = SIMD::RayBox4_SSE(myOrgXf, myOrgYf, myOrgZf,
                                       myInvDirXf, myInvDirYf, myInvDirZf,
                                       myTMinf, myCurrentTf,
                                       boxMinXf, boxMinYf, boxMinZf,
                                       boxMaxXf, boxMaxYf, boxMaxZf);
      result |= (static_cast<uint32_t>(hitMask1) & 0xF);

      if (N > 4)
      {
        // Second 4 rays
        int hitMask2 = SIMD::RayBox4_SSE(myOrgXf + 4, myOrgYf + 4, myOrgZf + 4,
                                         myInvDirXf + 4, myInvDirYf + 4, myInvDirZf + 4,
                                         myTMinf + 4, myCurrentTf + 4,
                                         boxMinXf, boxMinYf, boxMinZf,
                                         boxMaxXf, boxMaxYf, boxMaxZf);
        result |= (static_cast<uint32_t>(hitMask2) << 4);
      }

      return result & activeMask;
    }
#endif

    // Scalar fallback
    uint32_t result = 0;
    for (int i = 0; i < packet.ValidCount; ++i)
    {
      if (!(activeMask & (1u << i)))
        continue;

      double tNear, tFar;
      if (SIMD::RayBoxScalar(packet.OrgX[i], packet.OrgY[i], packet.OrgZ[i],
                             packet.InvDirX[i], packet.InvDirY[i], packet.InvDirZ[i],
                             packet.TMin[i], myCurrentTf[i],
                             boxMin[0], boxMin[1], boxMin[2],
                             boxMax[0], boxMax[1], boxMax[2],
                             tNear, tFar))
      {
        result |= (1u << i);
      }
    }
    return result;
  }

  //! SIMD triangle test for all active rays
  void TestTriangleSIMD(Standard_Integer triIdx,
                        const BRepIntCurveSurface_RayPacket<N>& packet,
                        BRepIntCurveSurface_PacketHitResult<N>& result,
                        uint32_t activeMask)
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

    Standard_Integer originalTriIdx = elem[3];
    if (originalTriIdx < 0 || originalTriIdx >= static_cast<Standard_Integer>(myTriangleInfo->size()))
      return;

    const BVH_Vec3d& v0 = myTriBVH->Vertices[elem[0]];
    const BVH_Vec3d& v1 = myTriBVH->Vertices[elem[1]];
    const BVH_Vec3d& v2 = myTriBVH->Vertices[elem[2]];

    // Convert vertices to float
    float v0xf = static_cast<float>(v0[0]), v0yf = static_cast<float>(v0[1]), v0zf = static_cast<float>(v0[2]);
    float v1xf = static_cast<float>(v1[0]), v1yf = static_cast<float>(v1[1]), v1zf = static_cast<float>(v1[2]);
    float v2xf = static_cast<float>(v2[0]), v2yf = static_cast<float>(v2[1]), v2zf = static_cast<float>(v2[2]);

    alignas(32) float outT[N], outU[N], outV[N];
    int hitMask = 0;

#if defined(OCCT_RT_HAS_AVX) && N >= 8
    if (myUseSIMD && N >= 8)
    {
      hitMask = SIMD::RayTriangle8_AVX(myOrgXf, myOrgYf, myOrgZf,
                                       myDirXf, myDirYf, myDirZf,
                                       v0xf, v0yf, v0zf,
                                       v1xf, v1yf, v1zf,
                                       v2xf, v2yf, v2zf,
                                       outT, outU, outV,
                                       myCurrentTf);

      hitMask &= static_cast<int>(activeMask);
      myTriangleTestCount += __builtin_popcount(static_cast<unsigned int>(activeMask));

      // Process hits
      for (int i = 0; i < 8; ++i)
      {
        if (hitMask & (1 << i))
        {
          // Update result
          result.HitT[i] = static_cast<Standard_Real>(outT[i]);
          result.BaryU[i] = static_cast<Standard_Real>(outU[i]);
          result.BaryV[i] = static_cast<Standard_Real>(outV[i]);
          result.TriIdx[i] = originalTriIdx;
          result.FaceIdx[i] = (*myTriangleInfo)[originalTriIdx].FaceIndex;
          result.HitMask |= (1u << i);

          // Update current T for early termination
          myCurrentTf[i] = outT[i];
        }
      }
      return;
    }
#endif

#if defined(OCCT_RT_HAS_SSE) && N >= 4
    if (myUseSIMD && N >= 4)
    {
      // First 4 rays
      int hitMask1 = SIMD::RayTriangle4_SSE(myOrgXf, myOrgYf, myOrgZf,
                                            myDirXf, myDirYf, myDirZf,
                                            v0xf, v0yf, v0zf,
                                            v1xf, v1yf, v1zf,
                                            v2xf, v2yf, v2zf,
                                            outT, outU, outV,
                                            myCurrentTf);

      myTriangleTestCount += __builtin_popcount(static_cast<unsigned int>(activeMask & 0xF));

      for (int i = 0; i < 4; ++i)
      {
        if ((hitMask1 & (1 << i)) && (activeMask & (1u << i)))
        {
          result.HitT[i] = static_cast<Standard_Real>(outT[i]);
          result.BaryU[i] = static_cast<Standard_Real>(outU[i]);
          result.BaryV[i] = static_cast<Standard_Real>(outV[i]);
          result.TriIdx[i] = originalTriIdx;
          result.FaceIdx[i] = (*myTriangleInfo)[originalTriIdx].FaceIndex;
          result.HitMask |= (1u << i);
          myCurrentTf[i] = outT[i];
        }
      }

      if (N > 4)
      {
        // Second 4 rays
        int hitMask2 = SIMD::RayTriangle4_SSE(myOrgXf + 4, myOrgYf + 4, myOrgZf + 4,
                                              myDirXf + 4, myDirYf + 4, myDirZf + 4,
                                              v0xf, v0yf, v0zf,
                                              v1xf, v1yf, v1zf,
                                              v2xf, v2yf, v2zf,
                                              outT + 4, outU + 4, outV + 4,
                                              myCurrentTf + 4);

        myTriangleTestCount += __builtin_popcount(static_cast<unsigned int>((activeMask >> 4) & 0xF));

        for (int i = 4; i < 8; ++i)
        {
          if ((hitMask2 & (1 << (i - 4))) && (activeMask & (1u << i)))
          {
            result.HitT[i] = static_cast<Standard_Real>(outT[i]);
            result.BaryU[i] = static_cast<Standard_Real>(outU[i]);
            result.BaryV[i] = static_cast<Standard_Real>(outV[i]);
            result.TriIdx[i] = originalTriIdx;
            result.FaceIdx[i] = (*myTriangleInfo)[originalTriIdx].FaceIndex;
            result.HitMask |= (1u << i);
            myCurrentTf[i] = outT[i];
          }
        }
      }
      return;
    }
#endif

    // Scalar fallback - Möller-Trumbore
    Standard_Real e1x = v1[0] - v0[0], e1y = v1[1] - v0[1], e1z = v1[2] - v0[2];
    Standard_Real e2x = v2[0] - v0[0], e2y = v2[1] - v0[1], e2z = v2[2] - v0[2];

    for (int i = 0; i < packet.ValidCount; ++i)
    {
      if (!(activeMask & (1u << i)))
        continue;

      ++myTriangleTestCount;

      Standard_Real hx = packet.DirY[i] * e2z - packet.DirZ[i] * e2y;
      Standard_Real hy = packet.DirZ[i] * e2x - packet.DirX[i] * e2z;
      Standard_Real hz = packet.DirX[i] * e2y - packet.DirY[i] * e2x;

      Standard_Real a = e1x * hx + e1y * hy + e1z * hz;
      if (std::abs(a) < 1e-12)
        continue;

      Standard_Real f  = 1.0 / a;
      Standard_Real sx = packet.OrgX[i] - v0[0];
      Standard_Real sy = packet.OrgY[i] - v0[1];
      Standard_Real sz = packet.OrgZ[i] - v0[2];

      Standard_Real u = f * (sx * hx + sy * hy + sz * hz);
      if (u < 0.0 || u > 1.0)
        continue;

      Standard_Real qx = sy * e1z - sz * e1y;
      Standard_Real qy = sz * e1x - sx * e1z;
      Standard_Real qz = sx * e1y - sy * e1x;

      Standard_Real v = f * (packet.DirX[i] * qx + packet.DirY[i] * qy + packet.DirZ[i] * qz);
      if (v < 0.0 || u + v > 1.0)
        continue;

      Standard_Real t = f * (e2x * qx + e2y * qy + e2z * qz);
      if (t > 1e-12 && t < result.HitT[i])
      {
        result.HitT[i]    = t;
        result.BaryU[i]   = u;
        result.BaryV[i]   = v;
        result.TriIdx[i]  = originalTriIdx;
        result.FaceIdx[i] = (*myTriangleInfo)[originalTriIdx].FaceIndex;
        result.HitMask |= (1u << i);
        myCurrentTf[i] = static_cast<float>(t);
      }
    }
  }

  std::vector<StackEntry>                              myStack;
  BRepIntCurveSurface_TriBVH*                          myTriBVH;
  const std::vector<BRepIntCurveSurface_TriangleInfo>* myTriangleInfo;
  Standard_Integer                                     myNodeTestCount;
  Standard_Integer                                     myTriangleTestCount;
  Standard_Integer                                     myFrustumCulled;
  bool                                                 myUseSIMD;
};
} // namespace

//=================================================================================================

void BRepIntCurveSurface_InterBVH::PerformBatchCoherent(
  const NCollection_Array1<gp_Lin>&                  theRays,
  NCollection_Array1<BRepIntCurveSurface_HitResult>& theResults,
  const Standard_Integer                             theNumThreads)
{
  (void)theNumThreads;

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

  if (myTriBVH.IsNull())
  {
    std::cerr << "Error: Triangle BVH not built - shape must be tessellated." << std::endl;
    return;
  }

  std::cout << "  [Coherent] Packet size: " << myPacketSize << ", Ray sorting: "
            << (myEnableRaySorting ? "enabled" : "disabled") << std::endl;

  // Sort rays and create packets
  BRepIntCurveSurface_RaySorter sorter;

  if (myEnableRaySorting)
  {
    // Sort by direction octant
    sorter.SortByOctant(theRays);

    // Get scene bounds for spatial sorting
    Bnd_Box sceneBox;
    for (Standard_Integer i = 1; i <= myFaces.Extent(); ++i)
    {
      Bnd_Box faceBox;
      BRepBndLib::Add(myFaces.FindKey(i), faceBox);
      sceneBox.Add(faceBox);
    }

    Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
    sceneBox.Get(xMin, yMin, zMin, xMax, yMax, zMax);
    BVH_Vec3d sceneMin(xMin, yMin, zMin);
    BVH_Vec3d sceneMax(xMax, yMax, zMax);

    // Sort by spatial locality within each bin
    sorter.SortBySpatialLocality(theRays, sceneMin, sceneMax);
  }
  else
  {
    // Just sort by octant (faster)
    sorter.SortByOctant(theRays);
  }

  // Create 8-ray packets
  std::vector<RayPacket8> packets;
  sorter.CreatePackets<8>(theRays, packets);

  std::cout << "  [Coherent] Created " << packets.size() << " packets from " << nRays << " rays" << std::endl;

  // Thread-local stats
  struct ThreadLocalStats
  {
    long long faceTests        = 0;
    long long newtonIters      = 0;
    long long newtonFailures   = 0;
    long long refinementTimeNs = 0;
    long long nodeTests        = 0;
    long long triangleTests    = 0;
    long long frustumCulled    = 0;
  };

  // Thread-local surface adaptors
  using ThreadLocalSurfaces = std::vector<Handle(Adaptor3d_Surface)>;

  // Lambda to create thread-local surface copies
  auto createLocalSurfaces = [&]() -> ThreadLocalSurfaces {
    ThreadLocalSurfaces localSurfaces;
    localSurfaces.reserve(mySurfaceAdaptors.size());
    for (const auto& adaptor : mySurfaceAdaptors)
    {
      localSurfaces.push_back(adaptor->ShallowCopy());
    }
    return localSurfaces;
  };

  // Process packets with PERSISTENT traverser (reused across packets)
  ThreadLocalStats  totalStats;
  ThreadLocalSurfaces localSurfaces = createLocalSurfaces();

  // Check if we should use Embree packet API
  bool useEmbreePackets = false;
#ifdef OCCT_USE_EMBREE
  useEmbreePackets = (myBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD4 ||
                      myBackend == BRepIntCurveSurface_BVHBackend::Embree_SIMD8) &&
                     myEmbreeScene != nullptr;
  if (useEmbreePackets)
  {
    std::cout << "  [Coherent] Using Embree packet API for sorted rays" << std::endl;
  }
#endif

  // Create persistent traverser - reuse across all packets for better cache efficiency
  BRepIntCurveSurface_PacketTraverser<8> traverser;
  traverser.SetTriBVH(myTriBVH.get());
  traverser.SetTriangleInfo(&myTriangleInfo);
  traverser.SetUseSIMD(true);  // Enable SIMD acceleration

  for (size_t p = 0; p < packets.size(); ++p)
  {
    RayPacket8&       packet = packets[p];
    PacketHitResult8 packetResult;

#ifdef OCCT_USE_EMBREE
    if (useEmbreePackets)
    {
      // Use Embree packet API with sorted rays for maximum performance
      IntersectEmbreePacket8(myEmbreeScene, packet, packetResult);

      // Map Embree primID to triangle info (Embree returns triangle index directly)
      for (int i = 0; i < packet.ValidCount; ++i)
      {
        if (packetResult.HitMask & (1u << i))
        {
          Standard_Integer triIdx = packetResult.TriIdx[i];
          if (triIdx >= 0 && triIdx < static_cast<Standard_Integer>(myTriangleInfo.size()))
          {
            packetResult.FaceIdx[i] = myTriangleInfo[triIdx].FaceIndex;
          }
        }
      }
    }
    else
#endif
    {
      // Use OCCT BVH traverser with SIMD (reusing persistent traverser)
      traverser.TraversePacket(packet, packetResult);

      totalStats.nodeTests += traverser.GetNodeTestCount();
      totalStats.triangleTests += traverser.GetTriangleTestCount();
      totalStats.frustumCulled += traverser.GetFrustumCulled();
    }

    // Process hits and refine with Newton
    for (int i = 0; i < packet.ValidCount; ++i)
    {
      if (!(packetResult.HitMask & (1u << i)))
        continue;

      Standard_Integer rayIdx     = packet.RayIndices[i];
      Standard_Integer hitTriIdx  = packetResult.TriIdx[i];
      Standard_Integer hitFaceIdx = packetResult.FaceIdx[i];
      Standard_Real    hitT       = packetResult.HitT[i];
      Standard_Real    baryU      = packetResult.BaryU[i];
      Standard_Real    baryV      = packetResult.BaryV[i];

      if (hitTriIdx < 0 || hitTriIdx >= static_cast<Standard_Integer>(myTriangleInfo.size()))
        continue;
      if (hitFaceIdx < 0 || hitFaceIdx >= static_cast<Standard_Integer>(mySurfaceAdaptors.size()))
        continue;

      totalStats.faceTests++;

      // UV-guided Newton refinement
      auto t0 = std::chrono::high_resolution_clock::now();

      const BRepIntCurveSurface_TriangleInfo& triInfo = myTriangleInfo[hitTriIdx];
      Standard_Real                           baryW   = 1.0 - baryU - baryV;
      Standard_Real                           initU =
        baryW * triInfo.UV0.X() + baryU * triInfo.UV1.X() + baryV * triInfo.UV2.X();
      Standard_Real initV =
        baryW * triInfo.UV0.Y() + baryU * triInfo.UV1.Y() + baryV * triInfo.UV2.Y();

      const Adaptor3d_Surface& aSurface = *localSurfaces[hitFaceIdx];
      Standard_Real            finalU   = initU;
      Standard_Real            finalV   = initV;
      Standard_Real            finalT   = 0.0;
      gp_Pnt                   finalPnt;
      Standard_Integer         iterCount = 0;

      gp_Lin aRay(gp_Pnt(packet.OrgX[i], packet.OrgY[i], packet.OrgZ[i]),
                  gp_Dir(packet.DirX[i], packet.DirY[i], packet.DirZ[i]));

      // Check if face is planar (skip Newton for planar faces)
      Standard_Boolean isPlanar = mySkipNewtonForPlanar
                                  && hitFaceIdx < static_cast<Standard_Integer>(myIsPlanarFace.size())
                                  && myIsPlanarFace[hitFaceIdx];

      NewtonResult newtonResult = NewtonResult::Converged;
      if (!isPlanar)
      {
        newtonResult = RefineIntersectionNewton(aSurface,
                                                aRay.Location(),
                                                aRay.Direction(),
                                                finalU,
                                                finalV,
                                                finalT,
                                                finalPnt,
                                                iterCount,
                                                myNewtonTolerance,
                                                myNewtonMaxIter);
      }
      else
      {
        // Planar face - use triangle intersection directly
        finalT = hitT;
        finalPnt = aRay.Location().Translated(hitT * gp_Vec(aRay.Direction()));
      }

      auto t1 = std::chrono::high_resolution_clock::now();
      totalStats.refinementTimeNs +=
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
      totalStats.newtonIters += iterCount;

      BRepIntCurveSurface_HitResult& aResult = theResults(rayIdx);
      aResult.IsValid                        = Standard_True;

      if (isPlanar || (newtonResult == NewtonResult::Converged && finalT >= 0.0))
      {
        aResult.Point = finalPnt;
        aResult.U     = isPlanar ? initU : finalU;
        aResult.V     = isPlanar ? initV : finalV;
        aResult.W     = isPlanar ? hitT : finalT;
      }
      else
      {
        totalStats.newtonFailures++;
        aResult.Point = aRay.Location().Translated(hitT * gp_Vec(aRay.Direction()));
        aResult.U     = initU;
        aResult.V     = initV;
        aResult.W     = hitT;
      }

      aResult.FaceIndex  = hitFaceIdx + 1;
      aResult.Transition = IntCurveSurface_In;
      aResult.State      = TopAbs_IN;

      // Compute surface normal (and optionally curvatures)
      gp_Pnt normPnt;
      gp_Vec dSdu, dSdv;
      gp_Vec normalVec;

      if (myComputeCurvature)
      {
        // Full D2 evaluation for curvatures
        gp_Vec d2Sdu2, d2Sdv2, d2Sduv;
        aSurface.D2(aResult.U, aResult.V, normPnt, dSdu, dSdv, d2Sdu2, d2Sdv2, d2Sduv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
          if (aFace.Orientation() == TopAbs_REVERSED)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);

          // Compute curvatures
          Standard_Real E = dSdu.Dot(dSdu);
          Standard_Real F = dSdu.Dot(dSdv);
          Standard_Real G = dSdv.Dot(dSdv);
          Standard_Real L = d2Sdu2.Dot(normalVec);
          Standard_Real M = d2Sduv.Dot(normalVec);
          Standard_Real Ncoef = d2Sdv2.Dot(normalVec);

          Standard_Real denom = E * G - F * F;
          if (std::abs(denom) > 1e-12)
          {
            aResult.GaussianCurvature = (L * Ncoef - M * M) / denom;
            aResult.MeanCurvature     = (E * Ncoef - 2.0 * F * M + G * L) / (2.0 * denom);
            Standard_Real disc =
              aResult.MeanCurvature * aResult.MeanCurvature - aResult.GaussianCurvature;
            if (disc >= 0.0)
            {
              Standard_Real sqrtDisc   = std::sqrt(disc);
              aResult.MinCurvature     = aResult.MeanCurvature - sqrtDisc;
              aResult.MaxCurvature     = aResult.MeanCurvature + sqrtDisc;
            }
          }
        }
      }
      else
      {
        // D1 only - just compute normal (much cheaper than D2)
        aSurface.D1(aResult.U, aResult.V, normPnt, dSdu, dSdv);
        normalVec = dSdu.Crossed(dSdv);
        Standard_Real normalMag = normalVec.Magnitude();

        if (normalMag > 1e-10)
        {
          normalVec.Normalize();
          const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(hitFaceIdx + 1));
          if (aFace.Orientation() == TopAbs_REVERSED)
            normalVec.Reverse();
          aResult.Normal = gp_Dir(normalVec);
        }
        else
        {
          aResult.Normal = gp_Dir(0, 0, 1);
        }
        // Curvatures remain at default 0
      }
    }
  }

  auto endTime = std::chrono::high_resolution_clock::now();
  double elapsedMs =
    std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() / 1000.0;

  // Count hits
  Standard_Integer hitCount = 0;
  for (Standard_Integer i = theResults.Lower(); i <= theResults.Upper(); ++i)
  {
    if (theResults(i).IsValid)
      ++hitCount;
  }

  double raysPerSec = (elapsedMs > 0.0) ? (nRays / (elapsedMs / 1000.0)) : 0.0;
  std::cout << "  [Coherent] Total time: " << std::fixed << std::setprecision(2) << (elapsedMs / 1000.0)
            << "s, " << static_cast<long long>(raysPerSec) << " rays/sec" << std::endl;
  std::cout << "  [Coherent] Node tests: " << totalStats.nodeTests
            << ", Triangle tests: " << totalStats.triangleTests
            << ", Frustum culled: " << totalStats.frustumCulled << std::endl;
  std::cout << "  [Coherent] Hits: " << hitCount << "/" << nRays << std::endl;

#ifdef OCCT_RT_HAS_AVX
  std::cout << "  [Coherent] SIMD: AVX enabled (8-way)" << std::endl;
#elif defined(OCCT_RT_HAS_SSE)
  std::cout << "  [Coherent] SIMD: SSE enabled (4-way)" << std::endl;
#else
  std::cout << "  [Coherent] SIMD: Scalar fallback" << std::endl;
#endif

  if (totalStats.newtonFailures > 0)
  {
    std::cout << "  [Coherent] Newton failures: " << totalStats.newtonFailures << std::endl;
  }
}
