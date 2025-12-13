// OCCT-RT Interactive Viewer - CAD File Loader Implementation
//
// Copyright (c) 2024 Andrea Pozzetti
// Licensed under LGPL 2.1

#include "Viewer_CADLoader.hxx"

#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Compound.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopAbs_ShapeEnum.hxx>

// STEP support
#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>

// IGES support
#include <IGESControl_Reader.hxx>

#include <algorithm>
#include <set>
#include <functional>
#include <vector>

//=============================================================================
// Format Detection
//=============================================================================

Viewer_FileFormat Viewer_CADLoader::DetectFormat(const std::string& thePath)
{
  // Find the last dot
  size_t dotPos = thePath.rfind('.');
  if (dotPos == std::string::npos)
    return Viewer_FileFormat::Unknown;

  // Extract extension and convert to lowercase
  std::string ext = thePath.substr(dotPos + 1);
  std::transform(ext.begin(), ext.end(), ext.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  // Match extension to format
  if (ext == "brep" || ext == "brp")
    return Viewer_FileFormat::BREP;
  if (ext == "step" || ext == "stp")
    return Viewer_FileFormat::STEP;
  if (ext == "iges" || ext == "igs")
    return Viewer_FileFormat::IGES;

  return Viewer_FileFormat::Unknown;
}

std::string Viewer_CADLoader::FormatName(Viewer_FileFormat theFormat)
{
  switch (theFormat)
  {
    case Viewer_FileFormat::BREP: return "BREP";
    case Viewer_FileFormat::STEP: return "STEP";
    case Viewer_FileFormat::IGES: return "IGES";
    default: return "Unknown";
  }
}

//=============================================================================
// Loading
//=============================================================================

Standard_Boolean Viewer_CADLoader::Load(const std::string& thePath,
                                         TopoDS_Shape& theShape,
                                         std::string& theError)
{
  Viewer_FileFormat format = DetectFormat(thePath);
  if (format == Viewer_FileFormat::Unknown)
  {
    theError = "Unknown file format. Supported: .brep, .brp, .step, .stp, .iges, .igs";
    return Standard_False;
  }

  return Load(thePath, format, theShape, theError);
}

Standard_Boolean Viewer_CADLoader::Load(const std::string& thePath,
                                         Viewer_FileFormat theFormat,
                                         TopoDS_Shape& theShape,
                                         std::string& theError)
{
  switch (theFormat)
  {
    case Viewer_FileFormat::BREP:
      return LoadBREP(thePath, theShape, theError);
    case Viewer_FileFormat::STEP:
      return LoadSTEP(thePath, theShape, theError);
    case Viewer_FileFormat::IGES:
      return LoadIGES(thePath, theShape, theError);
    default:
      theError = "Unknown file format";
      return Standard_False;
  }
}

//=============================================================================
// BREP Loading
//=============================================================================

Standard_Boolean Viewer_CADLoader::LoadBREP(const std::string& thePath,
                                             TopoDS_Shape& theShape,
                                             std::string& theError)
{
  BRep_Builder builder;
  if (!BRepTools::Read(theShape, thePath.c_str(), builder))
  {
    theError = "Failed to read BREP file: " + thePath;
    return Standard_False;
  }

  if (theShape.IsNull())
  {
    theError = "BREP file contains no shape: " + thePath;
    return Standard_False;
  }

  return Standard_True;
}

//=============================================================================
// STEP Loading
//=============================================================================

Standard_Boolean Viewer_CADLoader::LoadSTEP(const std::string& thePath,
                                             TopoDS_Shape& theShape,
                                             std::string& theError)
{
  STEPControl_Reader reader;
  IFSelect_ReturnStatus status = reader.ReadFile(thePath.c_str());

  if (status != IFSelect_RetDone)
  {
    theError = "Failed to read STEP file: " + thePath;
    switch (status)
    {
      case IFSelect_RetError:
        theError += " (syntax error)";
        break;
      case IFSelect_RetFail:
        theError += " (load failed)";
        break;
      case IFSelect_RetVoid:
        theError += " (file is empty)";
        break;
      default:
        break;
    }
    return Standard_False;
  }

  // Transfer all roots
  Standard_Integer nbRoots = reader.TransferRoots();
  if (nbRoots == 0)
  {
    theError = "No shapes could be transferred from STEP file: " + thePath;
    return Standard_False;
  }

  // Get the result
  if (reader.NbShapes() == 0)
  {
    theError = "No shapes found in STEP file: " + thePath;
    return Standard_False;
  }

  if (reader.NbShapes() == 1)
  {
    theShape = reader.Shape(1);
  }
  else
  {
    // Multiple shapes - combine into compound
    TopoDS_Compound compound;
    BRep_Builder builder;
    builder.MakeCompound(compound);
    for (Standard_Integer i = 1; i <= reader.NbShapes(); ++i)
    {
      builder.Add(compound, reader.Shape(i));
    }
    theShape = compound;
  }

  if (theShape.IsNull())
  {
    theError = "STEP file contains no valid geometry: " + thePath;
    return Standard_False;
  }

  return Standard_True;
}

//=============================================================================
// IGES Loading
//=============================================================================

Standard_Boolean Viewer_CADLoader::LoadIGES(const std::string& thePath,
                                             TopoDS_Shape& theShape,
                                             std::string& theError)
{
  IGESControl_Reader reader;
  IFSelect_ReturnStatus status = reader.ReadFile(thePath.c_str());

  if (status != IFSelect_RetDone)
  {
    theError = "Failed to read IGES file: " + thePath;
    switch (status)
    {
      case IFSelect_RetError:
        theError += " (syntax error)";
        break;
      case IFSelect_RetFail:
        theError += " (load failed)";
        break;
      case IFSelect_RetVoid:
        theError += " (file is empty)";
        break;
      default:
        break;
    }
    return Standard_False;
  }

  // Transfer all roots
  reader.TransferRoots();

  if (reader.NbShapes() == 0)
  {
    theError = "No shapes found in IGES file: " + thePath;
    return Standard_False;
  }

  theShape = reader.OneShape();

  if (theShape.IsNull())
  {
    theError = "IGES file contains no valid geometry: " + thePath;
    return Standard_False;
  }

  return Standard_True;
}

//=============================================================================
// Connected Component Analysis
//=============================================================================

Standard_Integer Viewer_CADLoader::CountConnectedComponents(const TopoDS_Shape& theShape)
{
  // Get all faces
  TopTools_IndexedMapOfShape faceMap;
  TopExp::MapShapes(theShape, TopAbs_FACE, faceMap);

  Standard_Integer nbFaces = faceMap.Extent();
  if (nbFaces == 0)
    return 0;
  if (nbFaces == 1)
    return 1;

  // Build edge-to-face adjacency map
  TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;
  TopExp::MapShapesAndAncestors(theShape, TopAbs_EDGE, TopAbs_FACE, edgeFaceMap);

  // Union-Find data structure
  std::vector<Standard_Integer> parent(nbFaces + 1);
  std::vector<Standard_Integer> rank(nbFaces + 1, 0);
  for (Standard_Integer i = 0; i <= nbFaces; ++i)
    parent[i] = i;

  // Find with path compression
  std::function<Standard_Integer(Standard_Integer)> find = [&](Standard_Integer x) -> Standard_Integer {
    if (parent[x] != x)
      parent[x] = find(parent[x]);
    return parent[x];
  };

  // Union by rank
  auto unite = [&](Standard_Integer x, Standard_Integer y) {
    Standard_Integer px = find(x);
    Standard_Integer py = find(y);
    if (px == py)
      return;
    if (rank[px] < rank[py])
      std::swap(px, py);
    parent[py] = px;
    if (rank[px] == rank[py])
      ++rank[px];
  };

  // Connect faces that share edges
  for (Standard_Integer i = 1; i <= edgeFaceMap.Extent(); ++i)
  {
    const TopTools_ListOfShape& faces = edgeFaceMap(i);
    if (faces.Extent() < 2)
      continue;

    // Get first face index
    TopTools_ListOfShape::Iterator it1(faces);
    Standard_Integer idx1 = faceMap.FindIndex(it1.Value());

    // Connect to all other faces sharing this edge
    for (it1.Next(); it1.More(); it1.Next())
    {
      Standard_Integer idx2 = faceMap.FindIndex(it1.Value());
      unite(idx1, idx2);
    }
  }

  // Count unique components
  std::set<Standard_Integer> components;
  for (Standard_Integer i = 1; i <= nbFaces; ++i)
  {
    components.insert(find(i));
  }

  return static_cast<Standard_Integer>(components.size());
}

Standard_Boolean Viewer_CADLoader::ValidateSingleComponent(const TopoDS_Shape& theShape,
                                                            std::string& theError)
{
  Standard_Integer nbComponents = CountConnectedComponents(theShape);

  if (nbComponents == 0)
  {
    theError = "Shape contains no faces";
    return Standard_False;
  }

  if (nbComponents != 1)
  {
    theError = "Shape has " + std::to_string(nbComponents)
             + " disconnected face components (expected 1)";
    return Standard_False;
  }

  return Standard_True;
}
