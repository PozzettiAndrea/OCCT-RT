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

#ifndef _BRepIntCurveSurface_ZEvaluator_HeaderFile
#define _BRepIntCurveSurface_ZEvaluator_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <BVH_BoxSet.hxx>
#include <BVH_BinnedBuilder.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <NCollection_Array1.hxx>
#include <NCollection_Sequence.hxx>
#include <gp_Pnt2d.hxx>
#include <Bnd_Box2d.hxx>
#include <Geom_Surface.hxx>

#include <vector>

//! Typedef for 2D BVH box set (XY bounding boxes with face indices)
typedef BVH_BoxSet<Standard_Real, 2, Standard_Integer> BRepIntCurveSurface_BVH2DBoxSet;

//! Structure to hold a Z evaluation result
struct BRepIntCurveSurface_ZResult
{
  Standard_Boolean IsValid;      //!< True if result is valid
  Standard_Real    Z;            //!< Z coordinate
  Standard_Real    U;            //!< U parameter on surface
  Standard_Real    V;            //!< V parameter on surface
  Standard_Integer FaceIndex;    //!< Index of face (1-based)

  BRepIntCurveSurface_ZResult()
    : IsValid(Standard_False),
      Z(0.0), U(0.0), V(0.0),
      FaceIndex(0)
  {}
};

//! 2D XY-to-Z surface evaluator using BVH acceleration.
//!
//! This class provides fast Z-height queries at given XY coordinates
//! by exploiting the fact that queries are axis-aligned (looking down Z).
//! Instead of full ray-surface intersection, it:
//! 1. Uses 2D BVH on XY projections of face bounding boxes
//! 2. Projects XY to UV parameters on candidate surfaces
//! 3. Classifies UV point against face boundaries
//! 4. Evaluates Z directly from surface
//!
//! This approach can be faster than ray intersection for axis-aligned queries.
//!
//! Usage:
//! @code
//!   BRepIntCurveSurface_ZEvaluator evaluator;
//!   evaluator.Load(myShape, 0.001);
//!
//!   // Single point query
//!   NCollection_Sequence<BRepIntCurveSurface_ZResult> results;
//!   evaluator.Evaluate(100.0, 50.0, results);
//!
//!   // Batch query
//!   NCollection_Array1<gp_Pnt2d> points(1, 1000);
//!   NCollection_Array1<NCollection_Sequence<BRepIntCurveSurface_ZResult>> batchResults;
//!   evaluator.EvaluateBatch(points, batchResults);
//! @endcode
class BRepIntCurveSurface_ZEvaluator
{
public:
  DEFINE_STANDARD_ALLOC

  //! Empty constructor
  Standard_EXPORT BRepIntCurveSurface_ZEvaluator();

  //! Destructor
  Standard_EXPORT ~BRepIntCurveSurface_ZEvaluator();

  //! Load a shape and build the 2D BVH acceleration structure.
  //! @param theShape Shape to evaluate (must contain faces)
  //! @param theTol Tolerance for classification
  Standard_EXPORT void Load(const TopoDS_Shape& theShape,
                            const Standard_Real theTol);

  //! Evaluate Z at a single XY point.
  //! Returns all Z values from faces that contain the XY point.
  //! @param theX X coordinate
  //! @param theY Y coordinate
  //! @param theResults Output sequence of Z results (one per overlapping face)
  Standard_EXPORT void Evaluate(const Standard_Real theX,
                                const Standard_Real theY,
                                NCollection_Sequence<BRepIntCurveSurface_ZResult>& theResults);

  //! Evaluate Z at multiple XY points (parallelized).
  //! @param thePoints Array of XY points to evaluate
  //! @param theResults Output array of result sequences (resized automatically)
  //! @param theNumThreads Number of threads (0 = auto)
  Standard_EXPORT void EvaluateBatch(
    const NCollection_Array1<gp_Pnt2d>& thePoints,
    NCollection_Array1<NCollection_Sequence<BRepIntCurveSurface_ZResult>>& theResults,
    const Standard_Integer theNumThreads = 0);

  //! Returns true if the shape has been loaded
  Standard_Boolean IsLoaded() const { return myIsLoaded; }

  //! Returns the number of faces in the loaded shape
  Standard_Integer NbFaces() const { return myFaces.Extent(); }

private:
  //! Evaluate a single XY point against a specific face
  Standard_Boolean EvaluateFace(const Standard_Integer theFaceIdx,
                                const Standard_Real theX,
                                const Standard_Real theY,
                                BRepIntCurveSurface_ZResult& theResult);

private:
  // 2D BVH containing face indices and their XY bounding boxes
  opencascade::handle<BRepIntCurveSurface_BVH2DBoxSet> myBVH2D;

  // Face data
  TopTools_IndexedMapOfShape myFaces;
  std::vector<Handle(Geom_Surface)> mySurfaces;
  std::vector<Bnd_Box2d> myXYBounds;  // XY bounding boxes for quick rejection

  // Tolerance
  Standard_Real myTolerance;

  // State flag
  Standard_Boolean myIsLoaded;
};

#endif // _BRepIntCurveSurface_ZEvaluator_HeaderFile
