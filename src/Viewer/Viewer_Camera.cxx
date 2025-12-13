// OCCT-RT Interactive Viewer - Camera Implementation
//
// Copyright (c) 2024 Andrea Pozzetti
// Licensed under LGPL 2.1

#include "Viewer_Camera.hxx"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//=============================================================================
// Constructor / Destructor
//=============================================================================

Viewer_Camera::Viewer_Camera()
  : myEye(0, 0, 10),
    myLookAt(0, 0, 0),
    myUp(0, 1, 0),
    myFOV(45.0),
    myAspectRatio(1.0),
    myIsRotating(Standard_False),
    myHasHome(Standard_False),
    myNeedsUpdate(Standard_True)
{
  myCurrentRotation.SetIdent();
  myRotationStart.SetIdent();
}

Viewer_Camera::~Viewer_Camera()
{
}

//=============================================================================
// Projection
//=============================================================================

Standard_Real Viewer_Camera::Distance() const
{
  return myEye.Distance(myLookAt);
}

void Viewer_Camera::UpdateBasis() const
{
  if (!myNeedsUpdate)
    return;

  // Forward = normalized(lookAt - eye)
  gp_Vec forward(myEye, myLookAt);
  if (forward.Magnitude() < 1e-10)
  {
    myForward = gp_Dir(0, 0, -1);
  }
  else
  {
    myForward = gp_Dir(forward);
  }

  // Right = forward x up (normalized)
  gp_Vec right = gp_Vec(myForward).Crossed(gp_Vec(myUp));
  if (right.Magnitude() < 1e-10)
  {
    // Up is parallel to forward, choose arbitrary perpendicular
    gp_Vec arbitrary = std::abs(myForward.X()) < 0.9 ? gp_Vec(1, 0, 0) : gp_Vec(0, 1, 0);
    right = gp_Vec(myForward).Crossed(arbitrary);
  }
  myRight = gp_Dir(right);

  // ViewUp = right x forward (orthogonalized)
  myViewUp = gp_Dir(gp_Vec(myRight).Crossed(gp_Vec(myForward)));

  myNeedsUpdate = Standard_False;
}

gp_Dir Viewer_Camera::Forward() const
{
  UpdateBasis();
  return myForward;
}

gp_Dir Viewer_Camera::Right() const
{
  UpdateBasis();
  return myRight;
}

gp_Dir Viewer_Camera::ViewUp() const
{
  UpdateBasis();
  return myViewUp;
}

//=============================================================================
// Ray Generation
//=============================================================================

gp_Lin Viewer_Camera::GenerateRay(Standard_Integer thePixelX,
                                   Standard_Integer thePixelY,
                                   Standard_Integer theWidth,
                                   Standard_Integer theHeight) const
{
  UpdateBasis();

  // Convert pixel to normalized device coordinates [-1, 1]
  // Add 0.5 to sample pixel center
  Standard_Real ndcX = (2.0 * (thePixelX + 0.5) / theWidth) - 1.0;
  Standard_Real ndcY = 1.0 - (2.0 * (thePixelY + 0.5) / theHeight); // Flip Y (0=top)

  // Compute view plane half-dimensions at unit distance
  Standard_Real fovRad = myFOV * M_PI / 180.0;
  Standard_Real halfHeight = std::tan(fovRad / 2.0);
  Standard_Real aspect = static_cast<Standard_Real>(theWidth) / theHeight;
  Standard_Real halfWidth = halfHeight * aspect;

  // Point on view plane
  Standard_Real viewX = ndcX * halfWidth;
  Standard_Real viewY = ndcY * halfHeight;

  // Ray direction in world space
  // direction = forward + viewX * right + viewY * viewUp (all at unit distance)
  gp_Vec direction = gp_Vec(myForward.XYZ())
                   + viewX * gp_Vec(myRight.XYZ())
                   + viewY * gp_Vec(myViewUp.XYZ());
  direction.Normalize();

  return gp_Lin(myEye, gp_Dir(direction));
}

void Viewer_Camera::GenerateAllRays(NCollection_Array1<gp_Lin>& theRays,
                                     Standard_Integer theWidth,
                                     Standard_Integer theHeight) const
{
  UpdateBasis();

  Standard_Integer totalRays = theWidth * theHeight;
  theRays.Resize(1, totalRays, Standard_False);

  // Precompute view plane parameters
  Standard_Real fovRad = myFOV * M_PI / 180.0;
  Standard_Real halfHeight = std::tan(fovRad / 2.0);
  Standard_Real aspect = static_cast<Standard_Real>(theWidth) / theHeight;
  Standard_Real halfWidth = halfHeight * aspect;

  gp_Vec forwardVec(myForward.XYZ());
  gp_Vec rightVec(myRight.XYZ());
  gp_Vec upVec(myViewUp.XYZ());

  Standard_Integer idx = 1;
  for (Standard_Integer y = 0; y < theHeight; ++y)
  {
    Standard_Real ndcY = 1.0 - (2.0 * (y + 0.5) / theHeight);
    Standard_Real viewY = ndcY * halfHeight;

    for (Standard_Integer x = 0; x < theWidth; ++x)
    {
      Standard_Real ndcX = (2.0 * (x + 0.5) / theWidth) - 1.0;
      Standard_Real viewX = ndcX * halfWidth;

      gp_Vec direction = forwardVec + viewX * rightVec + viewY * upVec;
      direction.Normalize();

      theRays(idx++) = gp_Lin(myEye, gp_Dir(direction));
    }
  }
}

//=============================================================================
// Trackball Rotation (Shoemake Arcball)
//=============================================================================

gp_Vec Viewer_Camera::ProjectToSphere(Standard_Real theX, Standard_Real theY) const
{
  // Project 2D point onto unit sphere using Shoemake's method
  // If inside sphere (d < r), project onto sphere
  // If outside, project onto hyperbolic sheet for smoother rotation at edges
  Standard_Real d2 = theX * theX + theY * theY;
  Standard_Real r2 = 1.0; // Trackball radius squared

  if (d2 < r2 / 2.0)
  {
    // Inside sphere - project directly onto sphere
    return gp_Vec(theX, theY, std::sqrt(r2 - d2));
  }
  else
  {
    // Outside sphere - project onto hyperbolic sheet
    // This gives smoother behavior at the edges
    Standard_Real z = (r2 / 2.0) / std::sqrt(d2);
    return gp_Vec(theX, theY, z);
  }
}

void Viewer_Camera::BeginRotation(Standard_Real theNormX, Standard_Real theNormY)
{
  myTrackballStart = ProjectToSphere(theNormX, theNormY);
  myRotationStart = myCurrentRotation;
  myIsRotating = Standard_True;
}

void Viewer_Camera::UpdateRotation(Standard_Real theNormX, Standard_Real theNormY)
{
  if (!myIsRotating)
    return;

  gp_Vec endPt = ProjectToSphere(theNormX, theNormY);

  // Compute rotation axis (cross product of start and end vectors)
  gp_Vec axis = myTrackballStart.Crossed(endPt);
  Standard_Real axisMag = axis.Magnitude();

  if (axisMag < 1e-10)
    return; // No significant rotation

  axis.Normalize();

  // Compute rotation angle from dot product
  Standard_Real dot = myTrackballStart.Dot(endPt);
  // Clamp to [-1, 1] to avoid numerical issues with acos
  dot = std::max(-1.0, std::min(1.0, dot / (myTrackballStart.Magnitude() * endPt.Magnitude())));
  Standard_Real angle = std::acos(dot);

  // Create delta rotation quaternion from axis and angle
  gp_Quaternion deltaRot;
  deltaRot.SetVectorAndAngle(axis, angle);

  // Combine with starting rotation: new = delta * start
  myCurrentRotation = deltaRot * myRotationStart;

  // Apply rotation to camera position around look-at point
  // Eye position = LookAt + rotation * (initial eye offset)
  if (myHasHome)
  {
    gp_Vec initialOffset(myHomeLookAt, myHomeEye);
    gp_Vec rotatedOffset = myCurrentRotation.Multiply(initialOffset);
    myEye = myLookAt.Translated(rotatedOffset);

    // Rotate up vector
    gp_Vec initialUp(myHomeUp.XYZ());
    gp_Vec rotatedUp = myCurrentRotation.Multiply(initialUp);
    myUp = gp_Dir(rotatedUp);
  }
  else
  {
    // No home set - rotate around current look-at
    Standard_Real dist = Distance();
    gp_Vec initialOffset(0, 0, dist); // Assume starting looking down -Z
    gp_Vec rotatedOffset = myCurrentRotation.Multiply(initialOffset);
    myEye = myLookAt.Translated(rotatedOffset);

    gp_Vec initialUp(0, 1, 0);
    gp_Vec rotatedUp = myCurrentRotation.Multiply(initialUp);
    myUp = gp_Dir(rotatedUp);
  }

  myNeedsUpdate = Standard_True;
}

void Viewer_Camera::EndRotation()
{
  myIsRotating = Standard_False;
}

//=============================================================================
// Navigation
//=============================================================================

void Viewer_Camera::Zoom(Standard_Real theDelta)
{
  UpdateBasis();

  Standard_Real dist = Distance();
  // Zoom factor: positive delta = zoom in (smaller distance)
  Standard_Real factor = std::exp(-theDelta);
  Standard_Real newDist = dist * factor;

  // Clamp minimum distance
  if (newDist < 0.001)
    newDist = 0.001;

  // Move eye along view direction
  gp_Vec offset(myLookAt, myEye);
  offset.Normalize();
  offset.Scale(newDist);
  myEye = myLookAt.Translated(offset);

  myNeedsUpdate = Standard_True;
}

void Viewer_Camera::Pan(Standard_Real theDeltaX, Standard_Real theDeltaY)
{
  UpdateBasis();

  // Scale pan by distance for consistent feel
  Standard_Real dist = Distance();
  Standard_Real scale = dist * 0.5;

  // Pan in view plane
  gp_Vec panOffset = theDeltaX * scale * gp_Vec(myRight.XYZ())
                   + theDeltaY * scale * gp_Vec(myViewUp.XYZ());

  myEye = myEye.Translated(panOffset);
  myLookAt = myLookAt.Translated(panOffset);

  myNeedsUpdate = Standard_True;
}

void Viewer_Camera::FitToBounds(const Bnd_Box& theBndBox, Standard_Real theMargin)
{
  if (theBndBox.IsVoid())
    return;

  Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
  theBndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  // Center of bounding box
  Standard_Real cx = (xmin + xmax) / 2.0;
  Standard_Real cy = (ymin + ymax) / 2.0;
  Standard_Real cz = (zmin + zmax) / 2.0;
  myLookAt = gp_Pnt(cx, cy, cz);

  // Bounding sphere radius
  Standard_Real dx = (xmax - xmin) * theMargin;
  Standard_Real dy = (ymax - ymin) * theMargin;
  Standard_Real dz = (zmax - zmin) * theMargin;
  Standard_Real radius = std::sqrt(dx*dx + dy*dy + dz*dz) / 2.0;

  // Distance needed to fit sphere in FOV
  Standard_Real fovRad = myFOV * M_PI / 180.0;
  Standard_Real dist = radius / std::sin(fovRad / 2.0);

  // Position camera at isometric-ish view (looking from +X+Y+Z direction)
  gp_Vec viewDir(1, 1, 1);
  viewDir.Normalize();
  myEye = myLookAt.Translated(dist * viewDir);

  // Up is Z
  myUp = gp_Dir(0, 0, 1);

  // Reset rotation state
  myCurrentRotation.SetIdent();

  // Save as home position
  SaveHomePosition();

  myNeedsUpdate = Standard_True;
}

void Viewer_Camera::SaveHomePosition()
{
  myHomeEye = myEye;
  myHomeLookAt = myLookAt;
  myHomeUp = myUp;
  myHasHome = Standard_True;
}

void Viewer_Camera::Reset()
{
  if (myHasHome)
  {
    myEye = myHomeEye;
    myLookAt = myHomeLookAt;
    myUp = myHomeUp;
    myCurrentRotation.SetIdent();
    myNeedsUpdate = Standard_True;
  }
}
