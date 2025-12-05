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

#ifndef _BRepIntCurveSurface_InterBVH_HeaderFile
#define _BRepIntCurveSurface_InterBVH_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Handle.hxx>

#include <BVH_LinearBuilder.hxx>
#include <BVH_Triangulation.hxx>
#include <gp_Lin.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt2d.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <IntCurveSurface_TransitionOnCurve.hxx>
#include <TopAbs_State.hxx>
#include <NCollection_Array1.hxx>
#include <BRepAdaptor_Surface.hxx>

#include <vector>

#ifdef OCCT_USE_EMBREE
#include <embree3/rtcore.h>
#endif

class BRepIntCurveSurface_InterBVH;

//! Backend selection for ray-triangle intersection
enum class BRepIntCurveSurface_BVHBackend {
  OCCT_BVH,           //!< OCCT's built-in BVH (fastest single-ray)
  Embree_Scalar,      //!< Embree rtcIntersect1 (single ray)
  Embree_SIMD4,       //!< Embree rtcIntersect4 (SSE, 4 rays at once)
  Embree_SIMD8        //!< Embree rtcIntersect8 (AVX, 8 rays at once)
};

//! Typedef for triangle BVH
typedef BVH_Triangulation<Standard_Real, 3> BRepIntCurveSurface_TriBVH;

//! Structure mapping a triangle to its source face and UV coordinates
struct BRepIntCurveSurface_TriangleInfo
{
  Standard_Integer FaceIndex;  //!< 0-based index into myFaces
  gp_Pnt2d UV0, UV1, UV2;      //!< UV coordinates of triangle vertices on the face
};

//! Structure to hold a single ray-surface hit result
struct BRepIntCurveSurface_HitResult
{
  Standard_Boolean IsValid;      //!< True if hit is valid
  gp_Pnt           Point;        //!< Hit point in 3D
  Standard_Real    U;            //!< U parameter on surface
  Standard_Real    V;            //!< V parameter on surface
  Standard_Real    W;            //!< Parameter on ray (distance from origin)
  Standard_Integer FaceIndex;    //!< Index of hit face (1-based)
  gp_Dir           Normal;       //!< Surface normal at hit point
  IntCurveSurface_TransitionOnCurve Transition; //!< Transition type
  TopAbs_State     State;        //!< State (IN or ON)

  // Curvature fields (computed when requested)
  Standard_Real    GaussianCurvature; //!< Gaussian curvature K = κ1 × κ2
  Standard_Real    MeanCurvature;     //!< Mean curvature H = (κ1 + κ2) / 2
  Standard_Real    MinCurvature;      //!< Minimum principal curvature κ1
  Standard_Real    MaxCurvature;      //!< Maximum principal curvature κ2

  BRepIntCurveSurface_HitResult()
    : IsValid(Standard_False),
      U(0.0), V(0.0), W(RealLast()),
      FaceIndex(0),
      Transition(IntCurveSurface_Tangent),
      State(TopAbs_UNKNOWN),
      GaussianCurvature(0.0),
      MeanCurvature(0.0),
      MinCurvature(0.0),
      MaxCurvature(0.0)
  {}
};

//! BVH-accelerated intersection between a curve (line) and a shape.
//!
//! This class provides the same functionality as BRepIntCurveSurface_Inter
//! but uses a Bounding Volume Hierarchy (BVH) for acceleration, giving
//! O(log n) performance instead of O(n) for shapes with many faces.
//!
//! Usage:
//! @code
//!   BRepIntCurveSurface_InterBVH inter;
//!   inter.Load(myShape, 0.001);  // Build BVH once
//!
//!   // Single ray query
//!   gp_Lin ray(gp_Pnt(0,0,10), gp_Dir(0,0,-1));
//!   inter.Perform(ray);
//!   if (inter.IsDone() && inter.NbPnt() > 0) {
//!     gp_Pnt hitPoint = inter.Pnt(1);
//!   }
//!
//!   // Or batch query for multiple rays
//!   NCollection_Array1<gp_Lin> rays(1, 1000);
//!   // ... fill rays ...
//!   NCollection_Array1<BRepIntCurveSurface_HitResult> results;
//!   inter.PerformBatch(rays, results);
//! @endcode
class BRepIntCurveSurface_InterBVH
{
public:
  DEFINE_STANDARD_ALLOC

  //! Empty constructor
  Standard_EXPORT BRepIntCurveSurface_InterBVH();

  //! Destructor
  Standard_EXPORT ~BRepIntCurveSurface_InterBVH();

  //! Load a shape and build the BVH acceleration structure.
  //! @param theShape Shape to intersect with (must contain faces, must be pre-tessellated)
  //! @param theTol Tolerance for intersection calculations
  //! @param theDeflection Linear deflection for tessellation (defaults to 0.1 if <= 0)
  //!        Controls vertex welding tolerance. Shape must already be tessellated before calling Load().
  Standard_EXPORT void Load(const TopoDS_Shape& theShape,
                            const Standard_Real theTol,
                            const Standard_Real theDeflection = 0.0);

  //! Perform intersection with a single ray (line).
  //! Results can be accessed via IsDone(), NbPnt(), Pnt(), etc.
  //! @param theLine Ray to intersect
  //! @param theMin Minimum parameter on ray (default 0)
  //! @param theMax Maximum parameter on ray (default infinite)
  Standard_EXPORT void Perform(const gp_Lin& theLine,
                               const Standard_Real theMin = 0.0,
                               const Standard_Real theMax = RealLast());

  //! Perform batch intersection with multiple rays (parallelized).
  //! @param theRays Array of rays to intersect
  //! @param theResults Output array of hit results (resized automatically)
  //! @param theNumThreads Number of threads (0 = auto)
  Standard_EXPORT void PerformBatch(const NCollection_Array1<gp_Lin>& theRays,
                                    NCollection_Array1<BRepIntCurveSurface_HitResult>& theResults,
                                    const Standard_Integer theNumThreads = 0);

  //! Perform batch intersection counting all hits per ray (not just closest).
  //! @param theRays Array of rays to intersect
  //! @param theHitCounts Output array of intersection counts per ray
  //! @param theNumThreads Number of threads (0 = auto)
  Standard_EXPORT void PerformBatchCount(const NCollection_Array1<gp_Lin>& theRays,
                                         NCollection_Array1<Standard_Integer>& theHitCounts,
                                         const Standard_Integer theNumThreads = 0);

  //! Returns true if intersection was performed successfully
  Standard_Boolean IsDone() const { return myIsDone; }

  //! Returns the number of intersection points found
  Standard_Integer NbPnt() const { return myNbPnt; }

  //! Returns the i-th intersection point (1-based index)
  Standard_EXPORT const gp_Pnt& Pnt(const Standard_Integer theIndex) const;

  //! Returns U parameter of i-th intersection on the surface
  Standard_EXPORT Standard_Real U(const Standard_Integer theIndex) const;

  //! Returns V parameter of i-th intersection on the surface
  Standard_EXPORT Standard_Real V(const Standard_Integer theIndex) const;

  //! Returns parameter of i-th intersection on the ray
  Standard_EXPORT Standard_Real W(const Standard_Integer theIndex) const;

  //! Returns the face containing the i-th intersection
  Standard_EXPORT const TopoDS_Face& Face(const Standard_Integer theIndex) const;

  //! Returns the surface normal at the i-th intersection point
  Standard_EXPORT gp_Dir Normal(const Standard_Integer theIndex) const;

  //! Returns the transition type at i-th intersection
  Standard_EXPORT IntCurveSurface_TransitionOnCurve Transition(const Standard_Integer theIndex) const;

  //! Returns the state (IN or ON) at i-th intersection
  Standard_EXPORT TopAbs_State State(const Standard_Integer theIndex) const;

  //! Returns true if the shape has been loaded and BVH built
  Standard_Boolean IsLoaded() const { return myIsLoaded; }

  //! Returns the number of faces in the loaded shape
  Standard_Integer NbFaces() const { return myFaces.Extent(); }

  //! Set the BVH backend for ray-triangle intersection
  //! @param theBackend Backend to use (OCCT_BVH, Embree_Scalar, Embree_SIMD4, Embree_SIMD8)
  void SetBackend(BRepIntCurveSurface_BVHBackend theBackend) { myBackend = theBackend; }

  //! Get current BVH backend
  BRepIntCurveSurface_BVHBackend GetBackend() const { return myBackend; }

  //! Enable/disable OpenMP parallelization for batch operations
  void SetUseOpenMP(Standard_Boolean theUse) { myUseOpenMP = theUse; }

  //! Check if OpenMP parallelization is enabled
  Standard_Boolean GetUseOpenMP() const { return myUseOpenMP; }

private:
  // Face data
  TopTools_IndexedMapOfShape myFaces;

  // Triangle BVH for tessellation-accelerated intersection
  opencascade::handle<BRepIntCurveSurface_TriBVH> myTriBVH;
  std::vector<BRepIntCurveSurface_TriangleInfo> myTriangleInfo;  // Maps triangle index to face + UV
  Standard_Boolean myUseTessellation;

  // Surface adaptors for fast UV-guided Newton refinement (used with tessellation BVH)
  std::vector<Handle(BRepAdaptor_Surface)> mySurfaceAdaptors;

  // Tolerance
  Standard_Real myTolerance;
  Standard_Real myDeflection;

  // State flags
  Standard_Boolean myIsLoaded;
  Standard_Boolean myIsDone;

  // Results storage for single-ray query
  std::vector<BRepIntCurveSurface_HitResult> myResults;
  Standard_Integer myNbPnt;

  // Runtime configuration
  BRepIntCurveSurface_BVHBackend myBackend;
  Standard_Boolean myUseOpenMP;

#ifdef OCCT_USE_EMBREE
  // Embree BVH acceleration
  RTCDevice myEmbreeDevice;
  RTCScene myEmbreeScene;
#endif
};

#endif // _BRepIntCurveSurface_InterBVH_HeaderFile
