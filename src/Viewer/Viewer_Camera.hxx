// OCCT-RT Interactive Viewer - Camera Class
// Perspective camera with quaternion-based trackball rotation
//
// Copyright (c) 2024 Andrea Pozzetti
// Licensed under LGPL 2.1

#ifndef _Viewer_Camera_HeaderFile
#define _Viewer_Camera_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>
#include <gp_Quaternion.hxx>
#include <Bnd_Box.hxx>
#include <NCollection_Array1.hxx>

//! Perspective camera with trackball rotation for interactive viewing.
//!
//! Features:
//! - Perspective projection with configurable FOV
//! - Quaternion-based trackball rotation (Shoemake arcball, no gimbal lock)
//! - Zoom via dolly (move along view direction)
//! - Pan in view plane
//! - Auto-fit to bounding box
//!
//! Coordinate system:
//! - Camera looks toward negative Z (standard OpenGL convention)
//! - Y is up by default
//!
//! Usage:
//! @code
//!   Viewer_Camera camera;
//!   camera.SetFOV(45.0);
//!   camera.FitToBounds(bndBox);
//!
//!   // Generate rays for rendering
//!   NCollection_Array1<gp_Lin> rays;
//!   camera.GenerateAllRays(rays, 800, 600);
//!
//!   // Interactive rotation
//!   camera.BeginRotation(normX, normY);  // On mouse down
//!   camera.UpdateRotation(normX, normY); // On mouse drag
//!   camera.EndRotation();                // On mouse up
//! @endcode
class Viewer_Camera
{
public:
  DEFINE_STANDARD_ALLOC

  //! Default constructor - camera at origin looking down -Z
  Standard_EXPORT Viewer_Camera();

  //! Destructor
  Standard_EXPORT ~Viewer_Camera();

  //=== Position and Orientation ===

  //! Set eye (camera) position
  void SetEye(const gp_Pnt& theEye) { myEye = theEye; myNeedsUpdate = Standard_True; }

  //! Get eye position
  const gp_Pnt& Eye() const { return myEye; }

  //! Set look-at target point
  void SetLookAt(const gp_Pnt& theLookAt) { myLookAt = theLookAt; myNeedsUpdate = Standard_True; }

  //! Get look-at target
  const gp_Pnt& LookAt() const { return myLookAt; }

  //! Set up vector
  void SetUp(const gp_Dir& theUp) { myUp = theUp; myNeedsUpdate = Standard_True; }

  //! Get up vector
  const gp_Dir& Up() const { return myUp; }

  //=== Projection Parameters ===

  //! Set vertical field of view in degrees
  void SetFOV(Standard_Real theFOVDegrees) { myFOV = theFOVDegrees; }

  //! Get vertical FOV in degrees
  Standard_Real FOV() const { return myFOV; }

  //! Get aspect ratio (width/height)
  Standard_Real AspectRatio() const { return myAspectRatio; }

  //! Get distance from eye to look-at point
  Standard_Real Distance() const;

  //=== Ray Generation ===

  //! Generate a single ray for a pixel coordinate
  //! @param thePixelX Pixel X coordinate (0 = left)
  //! @param thePixelY Pixel Y coordinate (0 = top)
  //! @param theWidth  Image width in pixels
  //! @param theHeight Image height in pixels
  //! @return Ray from camera through the pixel center
  Standard_EXPORT gp_Lin GenerateRay(Standard_Integer thePixelX,
                                      Standard_Integer thePixelY,
                                      Standard_Integer theWidth,
                                      Standard_Integer theHeight) const;

  //! Generate rays for all pixels in an image
  //! @param theRays Output array (resized to width*height)
  //! @param theWidth Image width
  //! @param theHeight Image height
  Standard_EXPORT void GenerateAllRays(NCollection_Array1<gp_Lin>& theRays,
                                        Standard_Integer theWidth,
                                        Standard_Integer theHeight) const;

  //=== Trackball Rotation (Shoemake Arcball) ===

  //! Begin trackball rotation - call on mouse button down
  //! @param theNormX Normalized X coordinate [-1, 1] (left to right)
  //! @param theNormY Normalized Y coordinate [-1, 1] (bottom to top)
  Standard_EXPORT void BeginRotation(Standard_Real theNormX, Standard_Real theNormY);

  //! Update trackball rotation - call on mouse move while dragging
  //! @param theNormX Current normalized X coordinate
  //! @param theNormY Current normalized Y coordinate
  Standard_EXPORT void UpdateRotation(Standard_Real theNormX, Standard_Real theNormY);

  //! End trackball rotation - call on mouse button up
  Standard_EXPORT void EndRotation();

  //! Check if currently in rotation mode
  Standard_Boolean IsRotating() const { return myIsRotating; }

  //=== Navigation ===

  //! Zoom by moving camera along view direction
  //! @param theDelta Positive = zoom in (closer), negative = zoom out
  Standard_EXPORT void Zoom(Standard_Real theDelta);

  //! Pan camera in the view plane
  //! @param theDeltaX Horizontal pan (positive = right)
  //! @param theDeltaY Vertical pan (positive = up)
  Standard_EXPORT void Pan(Standard_Real theDeltaX, Standard_Real theDeltaY);

  //! Fit camera to view a bounding box
  //! @param theBndBox Bounding box to fit
  //! @param theMargin Margin factor (1.0 = tight fit, 1.2 = 20% margin)
  Standard_EXPORT void FitToBounds(const Bnd_Box& theBndBox,
                                    Standard_Real theMargin = 1.2);

  //! Reset camera to initial state (after FitToBounds or manual setup)
  Standard_EXPORT void Reset();

  //! Store current state as the "home" position for Reset()
  Standard_EXPORT void SaveHomePosition();

  //=== View Direction Helpers ===

  //! Get forward direction (from eye toward look-at)
  gp_Dir Forward() const;

  //! Get right direction
  gp_Dir Right() const;

  //! Get orthogonalized up direction
  gp_Dir ViewUp() const;

private:
  //! Project a 2D point onto the virtual trackball sphere
  gp_Vec ProjectToSphere(Standard_Real theX, Standard_Real theY) const;

  //! Update cached basis vectors when camera parameters change
  void UpdateBasis() const;

private:
  // Camera position and orientation
  gp_Pnt myEye;
  gp_Pnt myLookAt;
  gp_Dir myUp;

  // Projection parameters
  Standard_Real myFOV;         //!< Vertical field of view in degrees
  Standard_Real myAspectRatio; //!< Width / height

  // Trackball state
  gp_Vec          myTrackballStart;   //!< Starting point on trackball sphere
  gp_Quaternion   myRotationStart;    //!< Rotation at start of drag
  gp_Quaternion   myCurrentRotation;  //!< Current accumulated rotation
  Standard_Boolean myIsRotating;      //!< True if currently rotating

  // Home position for Reset()
  gp_Pnt myHomeEye;
  gp_Pnt myHomeLookAt;
  gp_Dir myHomeUp;
  Standard_Boolean myHasHome;

  // Cached basis vectors (computed lazily)
  mutable gp_Dir myRight;
  mutable gp_Dir myViewUp;
  mutable gp_Dir myForward;
  mutable Standard_Boolean myNeedsUpdate;
};

#endif // _Viewer_Camera_HeaderFile
