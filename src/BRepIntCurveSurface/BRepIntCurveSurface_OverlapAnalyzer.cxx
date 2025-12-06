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

#include <BRepIntCurveSurface_OverlapAnalyzer.hxx>

#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

#include <algorithm>

//=======================================================================
// BRepIntCurveSurface_OverlapAnalyzer - Constructor
//=======================================================================
BRepIntCurveSurface_OverlapAnalyzer::BRepIntCurveSurface_OverlapAnalyzer() {}

//=======================================================================
// BRepIntCurveSurface_OverlapAnalyzer - Destructor
//=======================================================================
BRepIntCurveSurface_OverlapAnalyzer::~BRepIntCurveSurface_OverlapAnalyzer() {}

//=======================================================================
// Perform - Main analysis
//=======================================================================
void BRepIntCurveSurface_OverlapAnalyzer::Perform(const TopoDS_Shape& theShape)
{
  // Clear previous results
  myFaces.Clear();
  myXYBounds.clear();
  myOverlaps.clear();

  // Extract faces
  TopExp::MapShapes(theShape, TopAbs_FACE, myFaces);

  if (myFaces.IsEmpty())
    return;

  // Compute XY bounding boxes
  ComputeXYBounds();

  // Find overlapping pairs
  FindOverlaps();
}

//=======================================================================
// ComputeXYBounds - Compute XY bounding box for each face
//=======================================================================
void BRepIntCurveSurface_OverlapAnalyzer::ComputeXYBounds()
{
  myXYBounds.resize(myFaces.Extent());

  for (Standard_Integer i = 1; i <= myFaces.Extent(); ++i)
  {
    const TopoDS_Shape& face = myFaces(i);

    // Get 3D bounding box
    Bnd_Box box3d;
    BRepBndLib::Add(face, box3d);

    if (box3d.IsVoid())
      continue;

    // Extract XY bounds
    Standard_Real xMin, yMin, zMin, xMax, yMax, zMax;
    box3d.Get(xMin, yMin, zMin, xMax, yMax, zMax);

    myXYBounds[i - 1].Add(gp_Pnt2d(xMin, yMin));
    myXYBounds[i - 1].Add(gp_Pnt2d(xMax, yMax));
  }
}

//=======================================================================
// FindOverlaps - Find overlapping face pairs in XY
//=======================================================================
void BRepIntCurveSurface_OverlapAnalyzer::FindOverlaps()
{
  const Standard_Integer nFaces = myFaces.Extent();

  // Simple O(n²) check
  // Could be accelerated with spatial index for many faces (>1000)
  for (Standard_Integer i = 0; i < nFaces; ++i)
  {
    const Bnd_Box2d& box1 = myXYBounds[i];
    if (box1.IsVoid())
      continue;

    Standard_Real xMin1, yMin1, xMax1, yMax1;
    box1.Get(xMin1, yMin1, xMax1, yMax1);

    for (Standard_Integer j = i + 1; j < nFaces; ++j)
    {
      const Bnd_Box2d& box2 = myXYBounds[j];
      if (box2.IsVoid())
        continue;

      Standard_Real xMin2, yMin2, xMax2, yMax2;
      box2.Get(xMin2, yMin2, xMax2, yMax2);

      // Check for overlap
      Standard_Real overlapXMin = std::max(xMin1, xMin2);
      Standard_Real overlapYMin = std::max(yMin1, yMin2);
      Standard_Real overlapXMax = std::min(xMax1, xMax2);
      Standard_Real overlapYMax = std::min(yMax1, yMax2);

      if (overlapXMin < overlapXMax && overlapYMin < overlapYMax)
      {
        BRepIntCurveSurface_OverlapResult overlap;
        overlap.Face1Index = i + 1; // 1-based
        overlap.Face2Index = j + 1;
        overlap.OverlapRegion.Add(gp_Pnt2d(overlapXMin, overlapYMin));
        overlap.OverlapRegion.Add(gp_Pnt2d(overlapXMax, overlapYMax));
        overlap.OverlapArea = (overlapXMax - overlapXMin) * (overlapYMax - overlapYMin);

        myOverlaps.push_back(overlap);
      }
    }
  }

  // Sort by overlap area (largest first)
  std::sort(
    myOverlaps.begin(),
    myOverlaps.end(),
    [](const BRepIntCurveSurface_OverlapResult& a, const BRepIntCurveSurface_OverlapResult& b) {
      return a.OverlapArea > b.OverlapArea;
    });
}

//=======================================================================
// GetOverlappingPairs - Return pairs of overlapping face indices
//=======================================================================
void BRepIntCurveSurface_OverlapAnalyzer::GetOverlappingPairs(
  NCollection_Sequence<std::pair<Standard_Integer, Standard_Integer>>& thePairs) const
{
  thePairs.Clear();

  for (const auto& overlap : myOverlaps)
  {
    thePairs.Append(std::make_pair(overlap.Face1Index, overlap.Face2Index));
  }
}

//=======================================================================
// GetFaceXYBounds - Return XY bounding box for a face
//=======================================================================
Bnd_Box2d BRepIntCurveSurface_OverlapAnalyzer::GetFaceXYBounds(Standard_Integer theFaceIndex) const
{
  if (theFaceIndex < 1 || theFaceIndex > static_cast<Standard_Integer>(myXYBounds.size()))
    return Bnd_Box2d();

  return myXYBounds[theFaceIndex - 1];
}
