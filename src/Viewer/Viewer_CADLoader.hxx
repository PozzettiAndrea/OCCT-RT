// OCCT-RT Interactive Viewer - CAD File Loader
// Unified loader for BREP, STEP, and IGES formats
//
// Copyright (c) 2024 Andrea Pozzetti
// Licensed under LGPL 2.1

#ifndef _Viewer_CADLoader_HeaderFile
#define _Viewer_CADLoader_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <TopoDS_Shape.hxx>
#include <TCollection_AsciiString.hxx>

#include <string>

//! File format enumeration
enum class Viewer_FileFormat
{
  Unknown,
  BREP,
  STEP,
  IGES
};

//! Unified CAD file loader supporting BREP, STEP, and IGES formats.
//!
//! Features:
//! - Auto-detection of file format from extension
//! - Validation for single connected component
//! - Detailed error messages
//!
//! Usage:
//! @code
//!   TopoDS_Shape shape;
//!   std::string error;
//!   if (!Viewer_CADLoader::Load("model.step", shape, error)) {
//!     std::cerr << "Error: " << error << std::endl;
//!   }
//!   if (!Viewer_CADLoader::ValidateSingleComponent(shape, error)) {
//!     std::cerr << "Error: " << error << std::endl;
//!   }
//! @endcode
class Viewer_CADLoader
{
public:
  DEFINE_STANDARD_ALLOC

  //! Load a CAD file (auto-detects format from extension)
  //! @param thePath File path to load
  //! @param theShape Output shape
  //! @param theError Output error message if loading fails
  //! @return True if loading succeeded
  Standard_EXPORT static Standard_Boolean Load(const std::string& thePath,
                                                TopoDS_Shape& theShape,
                                                std::string& theError);

  //! Load a CAD file with explicit format
  //! @param thePath File path to load
  //! @param theFormat File format to use
  //! @param theShape Output shape
  //! @param theError Output error message if loading fails
  //! @return True if loading succeeded
  Standard_EXPORT static Standard_Boolean Load(const std::string& thePath,
                                                Viewer_FileFormat theFormat,
                                                TopoDS_Shape& theShape,
                                                std::string& theError);

  //! Detect file format from extension
  //! @param thePath File path
  //! @return Detected format or Unknown
  Standard_EXPORT static Viewer_FileFormat DetectFormat(const std::string& thePath);

  //! Get format name as string
  //! @param theFormat Format enum
  //! @return Format name (e.g., "STEP", "IGES", "BREP")
  Standard_EXPORT static std::string FormatName(Viewer_FileFormat theFormat);

  //! Count connected components in a shape
  //! Uses Union-Find on faces sharing edges
  //! @param theShape Shape to analyze
  //! @return Number of face-connected components
  Standard_EXPORT static Standard_Integer CountConnectedComponents(const TopoDS_Shape& theShape);

  //! Validate that shape is a single connected component
  //! @param theShape Shape to validate
  //! @param theError Output error message if validation fails
  //! @return True if shape is a single connected component
  Standard_EXPORT static Standard_Boolean ValidateSingleComponent(const TopoDS_Shape& theShape,
                                                                   std::string& theError);

private:
  //! Load BREP file
  static Standard_Boolean LoadBREP(const std::string& thePath,
                                    TopoDS_Shape& theShape,
                                    std::string& theError);

  //! Load STEP file
  static Standard_Boolean LoadSTEP(const std::string& thePath,
                                    TopoDS_Shape& theShape,
                                    std::string& theError);

  //! Load IGES file
  static Standard_Boolean LoadIGES(const std::string& thePath,
                                    TopoDS_Shape& theShape,
                                    std::string& theError);
};

#endif // _Viewer_CADLoader_HeaderFile
