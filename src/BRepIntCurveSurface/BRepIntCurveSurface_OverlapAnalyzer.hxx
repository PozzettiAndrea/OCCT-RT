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

#ifndef _BRepIntCurveSurface_OverlapAnalyzer_HeaderFile
#define _BRepIntCurveSurface_OverlapAnalyzer_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <TopoDS_Shape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <NCollection_Sequence.hxx>
#include <Bnd_Box2d.hxx>

#include <vector>
#include <utility>

//! Result of face-to-face overlap analysis in XY projection
struct BRepIntCurveSurface_OverlapResult
{
  Standard_Integer Face1Index;    //!< First face index (1-based)
  Standard_Integer Face2Index;    //!< Second face index (1-based)
  Bnd_Box2d        OverlapRegion; //!< Overlapping XY region (bounding box intersection)
  Standard_Real    OverlapArea;   //!< Area of overlap region in XY

  BRepIntCurveSurface_OverlapResult()
      : Face1Index(0),
        Face2Index(0),
        OverlapArea(0.0)
  {
  }
};

//! Analyzer for detecting face overlaps in XY projection.
//!
//! This class analyzes a shape to find pairs of faces whose XY bounding boxes
//! overlap. This is useful for determining if multiple Z values may exist at
//! a given XY coordinate (requiring full ray tracing) vs. having a simple
//! 1-to-1 XY→Z mapping.
//!
//! Usage:
//! @code
//!   BRepIntCurveSurface_OverlapAnalyzer analyzer;
//!   analyzer.Perform(myShape);
//!
//!   std::cout << "Overlapping face pairs: " << analyzer.NbOverlaps() << std::endl;
//!
//!   for (const auto& overlap : analyzer.Overlaps()) {
//!     std::cout << "Faces " << overlap.Face1Index << " and "
//!               << overlap.Face2Index << " overlap in XY\n";
//!   }
//! @endcode
class BRepIntCurveSurface_OverlapAnalyzer
{
public:
  DEFINE_STANDARD_ALLOC

  //! Empty constructor
  Standard_EXPORT BRepIntCurveSurface_OverlapAnalyzer();

  //! Destructor
  Standard_EXPORT ~BRepIntCurveSurface_OverlapAnalyzer();

  //! Perform overlap analysis on the shape.
  //! Finds all pairs of faces whose XY bounding boxes overlap.
  //! @param theShape Shape to analyze
  Standard_EXPORT void Perform(const TopoDS_Shape& theShape);

  //! Returns true if any faces overlap in XY projection
  Standard_Boolean HasAnyOverlap() const { return !myOverlaps.empty(); }

  //! Returns the number of faces in the shape
  Standard_Integer NbFaces() const { return myFaces.Extent(); }

  //! Returns the number of overlapping face pairs
  Standard_Integer NbOverlaps() const { return static_cast<Standard_Integer>(myOverlaps.size()); }

  //! Returns overlap results
  const std::vector<BRepIntCurveSurface_OverlapResult>& Overlaps() const { return myOverlaps; }

  //! Returns pairs of overlapping face indices
  Standard_EXPORT void GetOverlappingPairs(
    NCollection_Sequence<std::pair<Standard_Integer, Standard_Integer>>& thePairs) const;

  //! Returns the XY bounding box for a face (1-based index)
  Standard_EXPORT Bnd_Box2d GetFaceXYBounds(Standard_Integer theFaceIndex) const;

private:
  //! Compute XY bounding boxes for all faces
  void ComputeXYBounds();

  //! Find overlapping pairs (O(n²) simple approach)
  void FindOverlaps();

private:
  TopTools_IndexedMapOfShape                     myFaces;
  std::vector<Bnd_Box2d>                         myXYBounds; //!< XY bounds per face (0-based)
  std::vector<BRepIntCurveSurface_OverlapResult> myOverlaps;
};

#endif // _BRepIntCurveSurface_OverlapAnalyzer_HeaderFile
