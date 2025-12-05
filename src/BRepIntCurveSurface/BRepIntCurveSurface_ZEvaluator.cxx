// Created on: 2024-12-02
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

#include <BRepIntCurveSurface_ZEvaluator.hxx>

#include <BVH_Traverse.hxx>
#include <BVH_Tools.hxx>
#include <BRepBndLib.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepClass_FaceClassifier.hxx>
#include <BRep_Tool.hxx>
#include <Bnd_Box.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <Geom_Surface.hxx>
#include <GeomAPI_ProjectPointOnSurf.hxx>
#include <gp_Pnt.hxx>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <atomic>

#ifdef _OPENMP
#include <omp.h>
#endif

// Debug timing stats
namespace {
  std::atomic<long long> g_bvhQueryCount{0};
  std::atomic<long long> g_faceEvalCount{0};
  std::atomic<long long> g_projectionTimeNs{0};
  std::atomic<long long> g_classifyTimeNs{0};
}

namespace
{
  //! 2D Point-in-box traverser for BVH
  class ZEvaluator_PointTraverser
    : public BVH_Traverse<Standard_Real, 2,
                          BVH_BoxSet<Standard_Real, 2, Standard_Integer>,
                          Standard_Real>
  {
  public:
    typedef BVH_Box<Standard_Real, 2>::BVH_VecNt BVH_VecNt;

    ZEvaluator_PointTraverser()
      : myX(0.0), myY(0.0)
    {}

    void SetPoint(Standard_Real theX, Standard_Real theY)
    {
      myX = theX;
      myY = theY;
      myCandidates.clear();
    }

    //! Node rejection - test if point is inside bounding box
    Standard_Boolean RejectNode(const BVH_VecNt& theCornerMin,
                                const BVH_VecNt& theCornerMax,
                                Standard_Real& theMetric) const Standard_OVERRIDE
    {
      g_bvhQueryCount.fetch_add(1, std::memory_order_relaxed);

      // Simple 2D point-in-box test
      if (myX < theCornerMin[0] || myX > theCornerMax[0] ||
          myY < theCornerMin[1] || myY > theCornerMax[1])
      {
        return Standard_True; // Reject - point outside box
      }

      theMetric = 0.0;
      return Standard_False;
    }

    Standard_Boolean IsMetricBetter(const Standard_Real&,
                                    const Standard_Real&) const Standard_OVERRIDE
    {
      return Standard_False; // We want all candidates, not ordered
    }

    Standard_Boolean RejectMetric(const Standard_Real&) const Standard_OVERRIDE
    {
      return Standard_False; // Never reject - we want all overlapping faces
    }

    //! Leaf acceptance - collect face index
    Standard_Boolean Accept(const Standard_Integer theIndex,
                            const Standard_Real&) Standard_OVERRIDE
    {
      Standard_Integer aFaceIdx = this->myBVHSet->Element(theIndex);
      myCandidates.push_back(aFaceIdx);
      return Standard_False; // Continue traversal
    }

    const std::vector<Standard_Integer>& GetCandidates() const { return myCandidates; }

  private:
    Standard_Real myX, myY;
    std::vector<Standard_Integer> myCandidates;
  };
}

//=================================================================================================

BRepIntCurveSurface_ZEvaluator::BRepIntCurveSurface_ZEvaluator()
  : myTolerance(Precision::Confusion()),
    myIsLoaded(Standard_False)
{
}

//=================================================================================================

BRepIntCurveSurface_ZEvaluator::~BRepIntCurveSurface_ZEvaluator()
{
}

//=================================================================================================

void BRepIntCurveSurface_ZEvaluator::Load(const TopoDS_Shape& theShape,
                                           const Standard_Real theTol)
{
  myIsLoaded = Standard_False;
  myFaces.Clear();
  mySurfaces.clear();
  myXYBounds.clear();

  myTolerance = theTol;

  // Collect all faces from the shape
  TopExp_Explorer anExp(theShape, TopAbs_FACE);
  for (; anExp.More(); anExp.Next())
  {
    myFaces.Add(anExp.Current());
  }

  if (myFaces.IsEmpty())
    return;

  // Create 2D BVH builder
  opencascade::handle<BVH_BinnedBuilder<Standard_Real, 2>> aBuilder =
    new BVH_BinnedBuilder<Standard_Real, 2>(5, 32);

  // Create 2D BVH set
  myBVH2D = new BRepIntCurveSurface_BVH2DBoxSet(aBuilder);
  myBVH2D->SetSize(myFaces.Extent());

  // Pre-create surfaces and compute XY bounding boxes
  mySurfaces.resize(myFaces.Extent());
  myXYBounds.resize(myFaces.Extent());

  for (Standard_Integer i = 1; i <= myFaces.Extent(); ++i)
  {
    const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(i));

    // Get surface for this face
    mySurfaces[i - 1] = BRep_Tool::Surface(aFace);

    // Compute 3D bounding box
    Bnd_Box aBox3D;
    BRepBndLib::Add(aFace, aBox3D);
    aBox3D.Enlarge(myTolerance);

    // Extract XY bounds
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    aBox3D.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    myXYBounds[i - 1].Add(gp_Pnt2d(xmin, ymin));
    myXYBounds[i - 1].Add(gp_Pnt2d(xmax, ymax));

    // Add to 2D BVH
    BVH_Box<Standard_Real, 2> aBVH2DBox;
    aBVH2DBox.Add(BVH_Vec2d(xmin, ymin));
    aBVH2DBox.Add(BVH_Vec2d(xmax, ymax));

    myBVH2D->Add(i - 1, aBVH2DBox);  // Store 0-based index
  }

  // Build the BVH tree
  myBVH2D->Build();

  myIsLoaded = Standard_True;
}

//=================================================================================================

Standard_Boolean BRepIntCurveSurface_ZEvaluator::EvaluateFace(
  const Standard_Integer theFaceIdx,
  const Standard_Real theX,
  const Standard_Real theY,
  BRepIntCurveSurface_ZResult& theResult)
{
  g_faceEvalCount.fetch_add(1, std::memory_order_relaxed);

  theResult = BRepIntCurveSurface_ZResult();

  // Quick XY bounds check
  const Bnd_Box2d& xyBounds = myXYBounds[theFaceIdx];
  Standard_Real xmin, ymin, xmax, ymax;
  xyBounds.Get(xmin, ymin, xmax, ymax);
  if (theX < xmin - myTolerance || theX > xmax + myTolerance ||
      theY < ymin - myTolerance || theY > ymax + myTolerance)
  {
    return Standard_False;
  }

  const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(theFaceIdx + 1));
  const Handle(Geom_Surface)& aSurf = mySurfaces[theFaceIdx];

  // Project XY point to find UV parameters
  // We create a 3D point with Z=0 and project it onto the surface
  gp_Pnt aPoint3D(theX, theY, 0.0);

  auto t0 = std::chrono::high_resolution_clock::now();
  GeomAPI_ProjectPointOnSurf aProjector(aPoint3D, aSurf, myTolerance);
  auto t1 = std::chrono::high_resolution_clock::now();
  g_projectionTimeNs.fetch_add(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
                                std::memory_order_relaxed);

  if (aProjector.NbPoints() == 0)
    return Standard_False;

  // Get closest projection
  Standard_Real aU, aV;
  aProjector.LowerDistanceParameters(aU, aV);
  gp_Pnt aProjPoint = aProjector.NearestPoint();

  // Check if the projected point's XY is close to our query XY
  if (Abs(aProjPoint.X() - theX) > myTolerance ||
      Abs(aProjPoint.Y() - theY) > myTolerance)
  {
    // The surface doesn't pass through this XY at this UV
    return Standard_False;
  }

  gp_Pnt2d aUV(aU, aV);

  // Classify the UV point to check if it's inside the face boundaries
  auto t2 = std::chrono::high_resolution_clock::now();
  BRepClass_FaceClassifier aClassifier(aFace, aUV, myTolerance);
  auto t3 = std::chrono::high_resolution_clock::now();
  g_classifyTimeNs.fetch_add(std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count(),
                              std::memory_order_relaxed);

  TopAbs_State aState = aClassifier.State();
  if (aState != TopAbs_IN && aState != TopAbs_ON)
  {
    return Standard_False; // Point is outside face boundaries
  }

  // Success - fill result
  theResult.IsValid = Standard_True;
  theResult.Z = aProjPoint.Z();
  theResult.U = aUV.X();
  theResult.V = aUV.Y();
  theResult.FaceIndex = theFaceIdx + 1;

  return Standard_True;
}

//=================================================================================================

void BRepIntCurveSurface_ZEvaluator::Evaluate(
  const Standard_Real theX,
  const Standard_Real theY,
  NCollection_Sequence<BRepIntCurveSurface_ZResult>& theResults)
{
  theResults.Clear();

  if (!myIsLoaded)
    return;

  // Query 2D BVH for candidate faces
  ZEvaluator_PointTraverser aTraverser;
  aTraverser.SetBVHSet(myBVH2D.get());
  aTraverser.SetPoint(theX, theY);
  aTraverser.Select();

  // Evaluate each candidate face
  const std::vector<Standard_Integer>& aCandidates = aTraverser.GetCandidates();
  for (Standard_Integer aFaceIdx : aCandidates)
  {
    BRepIntCurveSurface_ZResult aResult;
    if (EvaluateFace(aFaceIdx, theX, theY, aResult))
    {
      theResults.Append(aResult);
    }
  }
}

//=================================================================================================

void BRepIntCurveSurface_ZEvaluator::EvaluateBatch(
  const NCollection_Array1<gp_Pnt2d>& thePoints,
  NCollection_Array1<NCollection_Sequence<BRepIntCurveSurface_ZResult>>& theResults,
  const Standard_Integer theNumThreads)
{
  if (!myIsLoaded)
  {
    theResults.Resize(thePoints.Lower(), thePoints.Upper(), Standard_False);
    return;
  }

  Standard_Integer nPoints = thePoints.Length();
  theResults.Resize(thePoints.Lower(), thePoints.Lower() + nPoints - 1, Standard_False);

  // Reset debug counters
  g_bvhQueryCount.store(0);
  g_faceEvalCount.store(0);
  g_projectionTimeNs.store(0);
  g_classifyTimeNs.store(0);

  auto startTime = std::chrono::high_resolution_clock::now();
  Standard_Integer lastProgress = 0;

#ifdef _OPENMP
  Standard_Integer nThreads = theNumThreads > 0 ? theNumThreads : omp_get_max_threads();
  #pragma omp parallel num_threads(nThreads)
  {
    #pragma omp for
    for (Standard_Integer i = 0; i < nPoints; ++i)
    {
      Standard_Integer idx = thePoints.Lower() + i;
      const gp_Pnt2d& aPt = thePoints(idx);

      NCollection_Sequence<BRepIntCurveSurface_ZResult>& results = theResults(idx);
      results.Clear();

      // Query 2D BVH for candidate faces
      ZEvaluator_PointTraverser aTraverser;
      aTraverser.SetBVHSet(myBVH2D.get());
      aTraverser.SetPoint(aPt.X(), aPt.Y());
      aTraverser.Select();

      // Evaluate each candidate face
      const std::vector<Standard_Integer>& aCandidates = aTraverser.GetCandidates();
      for (Standard_Integer aFaceIdx : aCandidates)
      {
        g_faceEvalCount.fetch_add(1, std::memory_order_relaxed);

        // Quick XY bounds check
        const Bnd_Box2d& xyBounds = myXYBounds[aFaceIdx];
        Standard_Real xmin, ymin, xmax, ymax;
        xyBounds.Get(xmin, ymin, xmax, ymax);
        if (aPt.X() < xmin - myTolerance || aPt.X() > xmax + myTolerance ||
            aPt.Y() < ymin - myTolerance || aPt.Y() > ymax + myTolerance)
        {
          continue;
        }

        const TopoDS_Face& aFace = TopoDS::Face(myFaces.FindKey(aFaceIdx + 1));
        const Handle(Geom_Surface)& aSurf = mySurfaces[aFaceIdx];

        // Project XY point to find UV parameters
        gp_Pnt aPoint3D(aPt.X(), aPt.Y(), 0.0);

        auto t0 = std::chrono::high_resolution_clock::now();
        GeomAPI_ProjectPointOnSurf aProjector(aPoint3D, aSurf, myTolerance);
        auto t1 = std::chrono::high_resolution_clock::now();
        g_projectionTimeNs.fetch_add(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count(),
                                      std::memory_order_relaxed);

        if (aProjector.NbPoints() == 0)
          continue;

        // Get closest projection
        Standard_Real aU, aV;
        aProjector.LowerDistanceParameters(aU, aV);
        gp_Pnt aProjPoint = aProjector.NearestPoint();

        // Check if the projected point's XY is close to our query XY
        if (Abs(aProjPoint.X() - aPt.X()) > myTolerance ||
            Abs(aProjPoint.Y() - aPt.Y()) > myTolerance)
        {
          continue;
        }

        gp_Pnt2d aUV(aU, aV);

        // Classify the UV point
        auto t2 = std::chrono::high_resolution_clock::now();
        BRepClass_FaceClassifier aClassifier(aFace, aUV, myTolerance);
        auto t3 = std::chrono::high_resolution_clock::now();
        g_classifyTimeNs.fetch_add(std::chrono::duration_cast<std::chrono::nanoseconds>(t3 - t2).count(),
                                    std::memory_order_relaxed);

        TopAbs_State aState = aClassifier.State();
        if (aState != TopAbs_IN && aState != TopAbs_ON)
        {
          continue;
        }

        // Success
        BRepIntCurveSurface_ZResult aResult;
        aResult.IsValid = Standard_True;
        aResult.Z = aProjPoint.Z();
        aResult.U = aU;
        aResult.V = aV;
        aResult.FaceIndex = aFaceIdx + 1;
        results.Append(aResult);
      }

      // Print progress
      if (omp_get_thread_num() == 0)
      {
        Standard_Integer currentProgress = (i + 1) * 100 / nPoints;
        if (currentProgress >= lastProgress + 5)
        {
          auto now = std::chrono::high_resolution_clock::now();
          double elapsedSec = std::chrono::duration<double>(now - startTime).count();
          std::cout << "  Progress: " << currentProgress << "% ("
                    << std::fixed << std::setprecision(1) << elapsedSec << "s)" << std::endl;
          lastProgress = currentProgress;
        }
      }
    }
  }
#else
  // Single-threaded fallback
  for (Standard_Integer i = 0; i < nPoints; ++i)
  {
    Standard_Integer idx = thePoints.Lower() + i;
    const gp_Pnt2d& aPt = thePoints(idx);
    Evaluate(aPt.X(), aPt.Y(), theResults(idx));

    Standard_Integer currentProgress = (i + 1) * 100 / nPoints;
    if (currentProgress >= lastProgress + 5)
    {
      auto now = std::chrono::high_resolution_clock::now();
      double elapsedSec = std::chrono::duration<double>(now - startTime).count();
      std::cout << "  Progress: " << currentProgress << "% ("
                << std::fixed << std::setprecision(1) << elapsedSec << "s)" << std::endl;
      lastProgress = currentProgress;
    }
  }
#endif

  // Print debug stats
  long long faceEvals = g_faceEvalCount.load();
  std::cout << "  [DEBUG-ZEval] BVH queries: " << g_bvhQueryCount.load()
            << " (" << (double)g_bvhQueryCount.load() / nPoints << " per point)" << std::endl;
  std::cout << "  [DEBUG-ZEval] Face evaluations: " << faceEvals
            << " (" << (double)faceEvals / nPoints << " per point)" << std::endl;
  if (faceEvals > 0)
  {
    std::cout << "  [DEBUG-ZEval] Projection time: "
              << g_projectionTimeNs.load() / 1000000.0 << " ms ("
              << (double)g_projectionTimeNs.load() / faceEvals / 1000.0 << " us per call)" << std::endl;
    std::cout << "  [DEBUG-ZEval] Classification time: "
              << g_classifyTimeNs.load() / 1000000.0 << " ms ("
              << (double)g_classifyTimeNs.load() / faceEvals / 1000.0 << " us per call)" << std::endl;
  }
}
