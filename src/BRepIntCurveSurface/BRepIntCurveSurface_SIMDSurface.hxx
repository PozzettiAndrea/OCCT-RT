// Created on: 2024-12-09
// Created by: Andrea Pozzetti (with Claude Code assistance)
// Copyright (c) 2024 OPEN CASCADE SAS
//
// SPDX-License-Identifier: LGPL-2.1-or-later
//
// Vectorized surface evaluation (D1 and D2) for elementary surface types.
// Processes 4 (u,v) points simultaneously using SIMD intrinsics.
//
// Supported surfaces: Plane, Cylinder, Sphere, Torus, Cone

#ifndef _BRepIntCurveSurface_SIMDSurface_HeaderFile
#define _BRepIntCurveSurface_SIMDSurface_HeaderFile

#include <Standard.hxx>
#include <gp_Ax3.hxx>
#include <GeomAbs_SurfaceType.hxx>

// SIMD intrinsics
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #define SIMD_SURFACE_NEON 1
  #include <arm_neon.h>
#elif defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
  #define SIMD_SURFACE_SSE 1
  #include <xmmintrin.h>
  #include <emmintrin.h>
#endif

#if defined(__AVX__)
  #define SIMD_SURFACE_AVX 1
  #include <immintrin.h>
#endif

#include <cmath>

//! Surface parameters for SIMD evaluation (pre-extracted from gp_Ax3 for efficiency)
struct SIMDSurfaceParams
{
  // Coordinate system (12 floats)
  alignas(16) float XDirX, XDirY, XDirZ;  // X direction
  alignas(16) float YDirX, YDirY, YDirZ;  // Y direction
  alignas(16) float ZDirX, ZDirY, ZDirZ;  // Z direction (axis)
  alignas(16) float LocX, LocY, LocZ;     // Location (origin)

  // Surface-specific parameters
  float Radius;        // For Cylinder, Sphere, Cone
  float MajorRadius;   // For Torus
  float MinorRadius;   // For Torus
  float SemiAngle;     // For Cone

  GeomAbs_SurfaceType Type;

  //! Initialize from gp_Ax3 and surface parameters
  void Init(const gp_Ax3& Pos, GeomAbs_SurfaceType theType,
            Standard_Real R = 0.0, Standard_Real MajR = 0.0,
            Standard_Real MinR = 0.0, Standard_Real Angle = 0.0)
  {
    Type = theType;
    Radius = static_cast<float>(R);
    MajorRadius = static_cast<float>(MajR);
    MinorRadius = static_cast<float>(MinR);
    SemiAngle = static_cast<float>(Angle);

    const gp_XYZ& XDir = Pos.XDirection().XYZ();
    const gp_XYZ& YDir = Pos.YDirection().XYZ();
    const gp_XYZ& ZDir = Pos.Direction().XYZ();
    const gp_XYZ& PLoc = Pos.Location().XYZ();

    XDirX = static_cast<float>(XDir.X());
    XDirY = static_cast<float>(XDir.Y());
    XDirZ = static_cast<float>(XDir.Z());
    YDirX = static_cast<float>(YDir.X());
    YDirY = static_cast<float>(YDir.Y());
    YDirZ = static_cast<float>(YDir.Z());
    ZDirX = static_cast<float>(ZDir.X());
    ZDirY = static_cast<float>(ZDir.Y());
    ZDirZ = static_cast<float>(ZDir.Z());
    LocX = static_cast<float>(PLoc.X());
    LocY = static_cast<float>(PLoc.Y());
    LocZ = static_cast<float>(PLoc.Z());
  }
};

//=============================================================================
// ARM NEON implementations (4-wide)
//=============================================================================
#ifdef SIMD_SURFACE_NEON

//! Compute D1 (point and first derivatives) for 4 points on a torus
//! @param params  Pre-initialized surface parameters
//! @param u       Array of 4 U values
//! @param v       Array of 4 V values
//! @param outPx,outPy,outPz   Output: surface points
//! @param outDuX,outDuY,outDuZ Output: dS/du derivatives
//! @param outDvX,outDvY,outDvZ Output: dS/dv derivatives
inline void TorusD1_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  // Load 4 U and V values
  float32x4_t vU = vld1q_f32(u);
  float32x4_t vV = vld1q_f32(v);

  // Compute cos/sin for U and V (scalar for now - vectorized trig is complex)
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
    cosV[i] = std::cos(v[i]);
    sinV[i] = std::sin(v[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vCosV = vld1q_f32(cosV);
  float32x4_t vSinV = vld1q_f32(sinV);

  // Load surface parameters
  float32x4_t vMajR = vdupq_n_f32(params.MajorRadius);
  float32x4_t vMinR = vdupq_n_f32(params.MinorRadius);

  // R1 = MinorRadius * cos(V)
  // R2 = MinorRadius * sin(V)
  // R  = MajorRadius + R1
  float32x4_t vR1 = vmulq_f32(vMinR, vCosV);
  float32x4_t vR2 = vmulq_f32(vMinR, vSinV);
  float32x4_t vR = vaddq_f32(vMajR, vR1);

  // A1 = R * CosU, A2 = R * SinU
  float32x4_t vA1 = vmulq_f32(vR, vCosU);
  float32x4_t vA2 = vmulq_f32(vR, vSinU);

  // A3 = R2 * CosU, A4 = R2 * SinU
  float32x4_t vA3 = vmulq_f32(vR2, vCosU);
  float32x4_t vA4 = vmulq_f32(vR2, vSinU);

  // Load direction vectors
  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // P = Location + A1*XDir + A2*YDir + R2*ZDir
  float32x4_t vPx = vaddq_f32(vLocX, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX), vR2, vZDirX));
  float32x4_t vPy = vaddq_f32(vLocY, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY), vR2, vZDirY));
  float32x4_t vPz = vaddq_f32(vLocZ, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ), vR2, vZDirZ));

  // Vu = -A2*XDir + A1*YDir
  float32x4_t vDuX = vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX);
  float32x4_t vDuY = vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY);
  float32x4_t vDuZ = vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ);

  // Vv = -A3*XDir - A4*YDir + R1*ZDir
  float32x4_t vDvX = vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirX)), vmulq_f32(vA4, vYDirX)), vR1, vZDirX);
  float32x4_t vDvY = vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirY)), vmulq_f32(vA4, vYDirY)), vR1, vZDirY);
  float32x4_t vDvZ = vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirZ)), vmulq_f32(vA4, vYDirZ)), vR1, vZDirZ);

  // Store results
  vst1q_f32(outPx, vPx);
  vst1q_f32(outPy, vPy);
  vst1q_f32(outPz, vPz);
  vst1q_f32(outDuX, vDuX);
  vst1q_f32(outDuY, vDuY);
  vst1q_f32(outDuZ, vDuZ);
  vst1q_f32(outDvX, vDvX);
  vst1q_f32(outDvY, vDvY);
  vst1q_f32(outDvZ, vDvZ);
}

//! Compute D2 (point, first and second derivatives) for 4 points on a torus
inline void TorusD2_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ,
  float* outDuuX, float* outDuuY, float* outDuuZ,
  float* outDvvX, float* outDvvY, float* outDvvZ,
  float* outDuvX, float* outDuvY, float* outDuvZ)
{
  // Compute cos/sin for U and V
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
    cosV[i] = std::cos(v[i]);
    sinV[i] = std::sin(v[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vCosV = vld1q_f32(cosV);
  float32x4_t vSinV = vld1q_f32(sinV);

  // Load surface parameters
  float32x4_t vMajR = vdupq_n_f32(params.MajorRadius);
  float32x4_t vMinR = vdupq_n_f32(params.MinorRadius);

  // R1 = MinorRadius * cos(V), R2 = MinorRadius * sin(V), R = MajorRadius + R1
  float32x4_t vR1 = vmulq_f32(vMinR, vCosV);
  float32x4_t vR2 = vmulq_f32(vMinR, vSinV);
  float32x4_t vR = vaddq_f32(vMajR, vR1);

  // A1 = R * CosU, A2 = R * SinU
  float32x4_t vA1 = vmulq_f32(vR, vCosU);
  float32x4_t vA2 = vmulq_f32(vR, vSinU);

  // A3 = R2 * CosU, A4 = R2 * SinU
  float32x4_t vA3 = vmulq_f32(vR2, vCosU);
  float32x4_t vA4 = vmulq_f32(vR2, vSinU);

  // A5 = R1 * CosU, A6 = R1 * SinU
  float32x4_t vA5 = vmulq_f32(vR1, vCosU);
  float32x4_t vA6 = vmulq_f32(vR1, vSinU);

  // Load direction vectors
  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // Som1 = A1*XDir + A2*YDir
  float32x4_t vSom1X = vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX);
  float32x4_t vSom1Y = vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY);
  float32x4_t vSom1Z = vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ);

  // R2Z = R2 * ZDir
  float32x4_t vR2ZX = vmulq_f32(vR2, vZDirX);
  float32x4_t vR2ZY = vmulq_f32(vR2, vZDirY);
  float32x4_t vR2ZZ = vmulq_f32(vR2, vZDirZ);

  // P = Location + Som1 + R2Z
  vst1q_f32(outPx, vaddq_f32(vaddq_f32(vLocX, vSom1X), vR2ZX));
  vst1q_f32(outPy, vaddq_f32(vaddq_f32(vLocY, vSom1Y), vR2ZY));
  vst1q_f32(outPz, vaddq_f32(vaddq_f32(vLocZ, vSom1Z), vR2ZZ));

  // Vu = -A2*XDir + A1*YDir
  vst1q_f32(outDuX, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX));
  vst1q_f32(outDuY, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY));
  vst1q_f32(outDuZ, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ));

  // Vv = -A3*XDir - A4*YDir + R1*ZDir
  vst1q_f32(outDvX, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirX)), vmulq_f32(vA4, vYDirX)), vR1, vZDirX));
  vst1q_f32(outDvY, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirY)), vmulq_f32(vA4, vYDirY)), vR1, vZDirY));
  vst1q_f32(outDvZ, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirZ)), vmulq_f32(vA4, vYDirZ)), vR1, vZDirZ));

  // Vuu = -Som1
  vst1q_f32(outDuuX, vnegq_f32(vSom1X));
  vst1q_f32(outDuuY, vnegq_f32(vSom1Y));
  vst1q_f32(outDuuZ, vnegq_f32(vSom1Z));

  // Vvv = -A5*XDir - A6*YDir - R2Z
  vst1q_f32(outDvvX, vsubq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA5, vXDirX)), vmulq_f32(vA6, vYDirX)), vR2ZX));
  vst1q_f32(outDvvY, vsubq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA5, vXDirY)), vmulq_f32(vA6, vYDirY)), vR2ZY));
  vst1q_f32(outDvvZ, vsubq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA5, vXDirZ)), vmulq_f32(vA6, vYDirZ)), vR2ZZ));

  // Vuv = A4*XDir - A3*YDir
  vst1q_f32(outDuvX, vsubq_f32(vmulq_f32(vA4, vXDirX), vmulq_f32(vA3, vYDirX)));
  vst1q_f32(outDuvY, vsubq_f32(vmulq_f32(vA4, vXDirY), vmulq_f32(vA3, vYDirY)));
  vst1q_f32(outDuvZ, vsubq_f32(vmulq_f32(vA4, vXDirZ), vmulq_f32(vA3, vYDirZ)));
}

//! Compute D1 for 4 points on a cylinder
inline void CylinderD1_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  // Compute cos/sin for U
  alignas(16) float cosU[4], sinU[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vV = vld1q_f32(v);

  float32x4_t vRadius = vdupq_n_f32(params.Radius);

  // A1 = Radius * cos(U), A2 = Radius * sin(U)
  float32x4_t vA1 = vmulq_f32(vRadius, vCosU);
  float32x4_t vA2 = vmulq_f32(vRadius, vSinU);

  // Load directions
  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // P = Location + A1*XDir + A2*YDir + V*ZDir
  vst1q_f32(outPx, vaddq_f32(vLocX, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX), vV, vZDirX)));
  vst1q_f32(outPy, vaddq_f32(vLocY, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY), vV, vZDirY)));
  vst1q_f32(outPz, vaddq_f32(vLocZ, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ), vV, vZDirZ)));

  // Vu = -A2*XDir + A1*YDir
  vst1q_f32(outDuX, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX));
  vst1q_f32(outDuY, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY));
  vst1q_f32(outDuZ, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ));

  // Vv = ZDir (constant)
  vst1q_f32(outDvX, vZDirX);
  vst1q_f32(outDvY, vZDirY);
  vst1q_f32(outDvZ, vZDirZ);
}

//! Compute D2 for 4 points on a cylinder
inline void CylinderD2_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ,
  float* outDuuX, float* outDuuY, float* outDuuZ,
  float* outDvvX, float* outDvvY, float* outDvvZ,
  float* outDuvX, float* outDuvY, float* outDuvZ)
{
  // Compute cos/sin for U
  alignas(16) float cosU[4], sinU[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vV = vld1q_f32(v);

  float32x4_t vRadius = vdupq_n_f32(params.Radius);
  float32x4_t vA1 = vmulq_f32(vRadius, vCosU);
  float32x4_t vA2 = vmulq_f32(vRadius, vSinU);

  // Load directions
  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // Som1 = A1*XDir + A2*YDir
  float32x4_t vSom1X = vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX);
  float32x4_t vSom1Y = vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY);
  float32x4_t vSom1Z = vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ);

  // P = Location + Som1 + V*ZDir
  vst1q_f32(outPx, vaddq_f32(vLocX, vmlaq_f32(vSom1X, vV, vZDirX)));
  vst1q_f32(outPy, vaddq_f32(vLocY, vmlaq_f32(vSom1Y, vV, vZDirY)));
  vst1q_f32(outPz, vaddq_f32(vLocZ, vmlaq_f32(vSom1Z, vV, vZDirZ)));

  // Vu = -A2*XDir + A1*YDir
  vst1q_f32(outDuX, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX));
  vst1q_f32(outDuY, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY));
  vst1q_f32(outDuZ, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ));

  // Vv = ZDir
  vst1q_f32(outDvX, vZDirX);
  vst1q_f32(outDvY, vZDirY);
  vst1q_f32(outDvZ, vZDirZ);

  // Vuu = -Som1
  vst1q_f32(outDuuX, vnegq_f32(vSom1X));
  vst1q_f32(outDuuY, vnegq_f32(vSom1Y));
  vst1q_f32(outDuuZ, vnegq_f32(vSom1Z));

  // Vvv = 0, Vuv = 0
  float32x4_t vZero = vdupq_n_f32(0.0f);
  vst1q_f32(outDvvX, vZero);
  vst1q_f32(outDvvY, vZero);
  vst1q_f32(outDvvZ, vZero);
  vst1q_f32(outDuvX, vZero);
  vst1q_f32(outDuvY, vZero);
  vst1q_f32(outDuvZ, vZero);
}

//! Compute D1 for 4 points on a sphere
inline void SphereD1_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  // Compute cos/sin for U and V
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
    cosV[i] = std::cos(v[i]);
    sinV[i] = std::sin(v[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vCosV = vld1q_f32(cosV);
  float32x4_t vSinV = vld1q_f32(sinV);

  float32x4_t vRadius = vdupq_n_f32(params.Radius);

  // R1 = Radius * cos(V), R2 = Radius * sin(V)
  float32x4_t vR1 = vmulq_f32(vRadius, vCosV);
  float32x4_t vR2 = vmulq_f32(vRadius, vSinV);

  // A1 = R1 * CosU, A2 = R1 * SinU
  float32x4_t vA1 = vmulq_f32(vR1, vCosU);
  float32x4_t vA2 = vmulq_f32(vR1, vSinU);

  // A3 = R2 * CosU, A4 = R2 * SinU
  float32x4_t vA3 = vmulq_f32(vR2, vCosU);
  float32x4_t vA4 = vmulq_f32(vR2, vSinU);

  // Load directions
  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // P = Location + A1*XDir + A2*YDir + R2*ZDir
  vst1q_f32(outPx, vaddq_f32(vLocX, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX), vR2, vZDirX)));
  vst1q_f32(outPy, vaddq_f32(vLocY, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY), vR2, vZDirY)));
  vst1q_f32(outPz, vaddq_f32(vLocZ, vmlaq_f32(vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ), vR2, vZDirZ)));

  // Vu = -A2*XDir + A1*YDir
  vst1q_f32(outDuX, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX));
  vst1q_f32(outDuY, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY));
  vst1q_f32(outDuZ, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ));

  // Vv = -A3*XDir - A4*YDir + R1*ZDir
  vst1q_f32(outDvX, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirX)), vmulq_f32(vA4, vYDirX)), vR1, vZDirX));
  vst1q_f32(outDvY, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirY)), vmulq_f32(vA4, vYDirY)), vR1, vZDirY));
  vst1q_f32(outDvZ, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirZ)), vmulq_f32(vA4, vYDirZ)), vR1, vZDirZ));
}

//! Compute D2 for 4 points on a sphere
inline void SphereD2_NEON4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ,
  float* outDuuX, float* outDuuY, float* outDuuZ,
  float* outDvvX, float* outDvvY, float* outDvvZ,
  float* outDuvX, float* outDuvY, float* outDuvZ)
{
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  for (int i = 0; i < 4; ++i) {
    cosU[i] = std::cos(u[i]);
    sinU[i] = std::sin(u[i]);
    cosV[i] = std::cos(v[i]);
    sinV[i] = std::sin(v[i]);
  }
  float32x4_t vCosU = vld1q_f32(cosU);
  float32x4_t vSinU = vld1q_f32(sinU);
  float32x4_t vCosV = vld1q_f32(cosV);
  float32x4_t vSinV = vld1q_f32(sinV);

  float32x4_t vRadius = vdupq_n_f32(params.Radius);
  float32x4_t vR1 = vmulq_f32(vRadius, vCosV);
  float32x4_t vR2 = vmulq_f32(vRadius, vSinV);

  float32x4_t vA1 = vmulq_f32(vR1, vCosU);
  float32x4_t vA2 = vmulq_f32(vR1, vSinU);
  float32x4_t vA3 = vmulq_f32(vR2, vCosU);
  float32x4_t vA4 = vmulq_f32(vR2, vSinU);

  float32x4_t vXDirX = vdupq_n_f32(params.XDirX);
  float32x4_t vXDirY = vdupq_n_f32(params.XDirY);
  float32x4_t vXDirZ = vdupq_n_f32(params.XDirZ);
  float32x4_t vYDirX = vdupq_n_f32(params.YDirX);
  float32x4_t vYDirY = vdupq_n_f32(params.YDirY);
  float32x4_t vYDirZ = vdupq_n_f32(params.YDirZ);
  float32x4_t vZDirX = vdupq_n_f32(params.ZDirX);
  float32x4_t vZDirY = vdupq_n_f32(params.ZDirY);
  float32x4_t vZDirZ = vdupq_n_f32(params.ZDirZ);
  float32x4_t vLocX = vdupq_n_f32(params.LocX);
  float32x4_t vLocY = vdupq_n_f32(params.LocY);
  float32x4_t vLocZ = vdupq_n_f32(params.LocZ);

  // Som1 = A1*XDir + A2*YDir
  float32x4_t vSom1X = vmlaq_f32(vmulq_f32(vA1, vXDirX), vA2, vYDirX);
  float32x4_t vSom1Y = vmlaq_f32(vmulq_f32(vA1, vXDirY), vA2, vYDirY);
  float32x4_t vSom1Z = vmlaq_f32(vmulq_f32(vA1, vXDirZ), vA2, vYDirZ);

  // R2Z
  float32x4_t vR2ZX = vmulq_f32(vR2, vZDirX);
  float32x4_t vR2ZY = vmulq_f32(vR2, vZDirY);
  float32x4_t vR2ZZ = vmulq_f32(vR2, vZDirZ);

  // P
  vst1q_f32(outPx, vaddq_f32(vaddq_f32(vLocX, vSom1X), vR2ZX));
  vst1q_f32(outPy, vaddq_f32(vaddq_f32(vLocY, vSom1Y), vR2ZY));
  vst1q_f32(outPz, vaddq_f32(vaddq_f32(vLocZ, vSom1Z), vR2ZZ));

  // Vu
  vst1q_f32(outDuX, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirX)), vA1, vYDirX));
  vst1q_f32(outDuY, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirY)), vA1, vYDirY));
  vst1q_f32(outDuZ, vmlaq_f32(vnegq_f32(vmulq_f32(vA2, vXDirZ)), vA1, vYDirZ));

  // Vv
  vst1q_f32(outDvX, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirX)), vmulq_f32(vA4, vYDirX)), vR1, vZDirX));
  vst1q_f32(outDvY, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirY)), vmulq_f32(vA4, vYDirY)), vR1, vZDirY));
  vst1q_f32(outDvZ, vmlaq_f32(vsubq_f32(vnegq_f32(vmulq_f32(vA3, vXDirZ)), vmulq_f32(vA4, vYDirZ)), vR1, vZDirZ));

  // Vuu = -Som1
  vst1q_f32(outDuuX, vnegq_f32(vSom1X));
  vst1q_f32(outDuuY, vnegq_f32(vSom1Y));
  vst1q_f32(outDuuZ, vnegq_f32(vSom1Z));

  // Vvv = -Som1 - R2Z
  vst1q_f32(outDvvX, vsubq_f32(vnegq_f32(vSom1X), vR2ZX));
  vst1q_f32(outDvvY, vsubq_f32(vnegq_f32(vSom1Y), vR2ZY));
  vst1q_f32(outDvvZ, vsubq_f32(vnegq_f32(vSom1Z), vR2ZZ));

  // Vuv = A4*XDir - A3*YDir
  vst1q_f32(outDuvX, vsubq_f32(vmulq_f32(vA4, vXDirX), vmulq_f32(vA3, vYDirX)));
  vst1q_f32(outDuvY, vsubq_f32(vmulq_f32(vA4, vXDirY), vmulq_f32(vA3, vYDirY)));
  vst1q_f32(outDuvZ, vsubq_f32(vmulq_f32(vA4, vXDirZ), vmulq_f32(vA3, vYDirZ)));
}

#endif // SIMD_SURFACE_NEON

//=============================================================================
// SSE implementations (4-wide) - for x86/x64
//=============================================================================
#ifdef SIMD_SURFACE_SSE

//! Helper: Compute sin/cos using scalar (SIMD sin/cos is complex)
inline void SinCos4_SSE(const float* angles, float* sinOut, float* cosOut)
{
  for (int i = 0; i < 4; ++i) {
    sinOut[i] = std::sin(angles[i]);
    cosOut[i] = std::cos(angles[i]);
  }
}

//! Compute D1 for 4 points on a torus (SSE)
inline void TorusD1_SSE4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  SinCos4_SSE(u, sinU, cosU);
  SinCos4_SSE(v, sinV, cosV);

  __m128 vCosU = _mm_load_ps(cosU);
  __m128 vSinU = _mm_load_ps(sinU);
  __m128 vCosV = _mm_load_ps(cosV);
  __m128 vSinV = _mm_load_ps(sinV);

  __m128 vMajR = _mm_set1_ps(params.MajorRadius);
  __m128 vMinR = _mm_set1_ps(params.MinorRadius);

  __m128 vR1 = _mm_mul_ps(vMinR, vCosV);
  __m128 vR2 = _mm_mul_ps(vMinR, vSinV);
  __m128 vR = _mm_add_ps(vMajR, vR1);

  __m128 vA1 = _mm_mul_ps(vR, vCosU);
  __m128 vA2 = _mm_mul_ps(vR, vSinU);
  __m128 vA3 = _mm_mul_ps(vR2, vCosU);
  __m128 vA4 = _mm_mul_ps(vR2, vSinU);

  __m128 vXDirX = _mm_set1_ps(params.XDirX);
  __m128 vXDirY = _mm_set1_ps(params.XDirY);
  __m128 vXDirZ = _mm_set1_ps(params.XDirZ);
  __m128 vYDirX = _mm_set1_ps(params.YDirX);
  __m128 vYDirY = _mm_set1_ps(params.YDirY);
  __m128 vYDirZ = _mm_set1_ps(params.YDirZ);
  __m128 vZDirX = _mm_set1_ps(params.ZDirX);
  __m128 vZDirY = _mm_set1_ps(params.ZDirY);
  __m128 vZDirZ = _mm_set1_ps(params.ZDirZ);
  __m128 vLocX = _mm_set1_ps(params.LocX);
  __m128 vLocY = _mm_set1_ps(params.LocY);
  __m128 vLocZ = _mm_set1_ps(params.LocZ);

  // P = Loc + A1*XDir + A2*YDir + R2*ZDir
  __m128 vPx = _mm_add_ps(vLocX, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirX), _mm_mul_ps(vA2, vYDirX)), _mm_mul_ps(vR2, vZDirX)));
  __m128 vPy = _mm_add_ps(vLocY, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirY), _mm_mul_ps(vA2, vYDirY)), _mm_mul_ps(vR2, vZDirY)));
  __m128 vPz = _mm_add_ps(vLocZ, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirZ), _mm_mul_ps(vA2, vYDirZ)), _mm_mul_ps(vR2, vZDirZ)));

  // Vu = -A2*XDir + A1*YDir
  __m128 vNeg = _mm_set1_ps(-1.0f);
  __m128 vDuX = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirX), _mm_mul_ps(vA1, vYDirX));
  __m128 vDuY = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirY), _mm_mul_ps(vA1, vYDirY));
  __m128 vDuZ = _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirZ), _mm_mul_ps(vA1, vYDirZ));

  // Vv = -A3*XDir - A4*YDir + R1*ZDir
  __m128 vDvX = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirX), _mm_mul_ps(vA4, vYDirX)), _mm_mul_ps(vR1, vZDirX));
  __m128 vDvY = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirY), _mm_mul_ps(vA4, vYDirY)), _mm_mul_ps(vR1, vZDirY));
  __m128 vDvZ = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirZ), _mm_mul_ps(vA4, vYDirZ)), _mm_mul_ps(vR1, vZDirZ));

  _mm_store_ps(outPx, vPx);
  _mm_store_ps(outPy, vPy);
  _mm_store_ps(outPz, vPz);
  _mm_store_ps(outDuX, vDuX);
  _mm_store_ps(outDuY, vDuY);
  _mm_store_ps(outDuZ, vDuZ);
  _mm_store_ps(outDvX, vDvX);
  _mm_store_ps(outDvY, vDvY);
  _mm_store_ps(outDvZ, vDvZ);
}

//! Compute D1 for 4 points on a cylinder (SSE)
inline void CylinderD1_SSE4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  alignas(16) float cosU[4], sinU[4];
  SinCos4_SSE(u, sinU, cosU);

  __m128 vCosU = _mm_load_ps(cosU);
  __m128 vSinU = _mm_load_ps(sinU);
  __m128 vV = _mm_loadu_ps(v);

  __m128 vRadius = _mm_set1_ps(params.Radius);
  __m128 vA1 = _mm_mul_ps(vRadius, vCosU);
  __m128 vA2 = _mm_mul_ps(vRadius, vSinU);

  __m128 vXDirX = _mm_set1_ps(params.XDirX);
  __m128 vXDirY = _mm_set1_ps(params.XDirY);
  __m128 vXDirZ = _mm_set1_ps(params.XDirZ);
  __m128 vYDirX = _mm_set1_ps(params.YDirX);
  __m128 vYDirY = _mm_set1_ps(params.YDirY);
  __m128 vYDirZ = _mm_set1_ps(params.YDirZ);
  __m128 vZDirX = _mm_set1_ps(params.ZDirX);
  __m128 vZDirY = _mm_set1_ps(params.ZDirY);
  __m128 vZDirZ = _mm_set1_ps(params.ZDirZ);
  __m128 vLocX = _mm_set1_ps(params.LocX);
  __m128 vLocY = _mm_set1_ps(params.LocY);
  __m128 vLocZ = _mm_set1_ps(params.LocZ);

  // P = Loc + A1*XDir + A2*YDir + V*ZDir
  _mm_store_ps(outPx, _mm_add_ps(vLocX, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirX), _mm_mul_ps(vA2, vYDirX)), _mm_mul_ps(vV, vZDirX))));
  _mm_store_ps(outPy, _mm_add_ps(vLocY, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirY), _mm_mul_ps(vA2, vYDirY)), _mm_mul_ps(vV, vZDirY))));
  _mm_store_ps(outPz, _mm_add_ps(vLocZ, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirZ), _mm_mul_ps(vA2, vYDirZ)), _mm_mul_ps(vV, vZDirZ))));

  // Vu = -A2*XDir + A1*YDir
  __m128 vNeg = _mm_set1_ps(-1.0f);
  _mm_store_ps(outDuX, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirX), _mm_mul_ps(vA1, vYDirX)));
  _mm_store_ps(outDuY, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirY), _mm_mul_ps(vA1, vYDirY)));
  _mm_store_ps(outDuZ, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirZ), _mm_mul_ps(vA1, vYDirZ)));

  // Vv = ZDir
  _mm_store_ps(outDvX, vZDirX);
  _mm_store_ps(outDvY, vZDirY);
  _mm_store_ps(outDvZ, vZDirZ);
}

//! Compute D1 for 4 points on a sphere (SSE)
inline void SphereD1_SSE4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  alignas(16) float cosU[4], sinU[4], cosV[4], sinV[4];
  SinCos4_SSE(u, sinU, cosU);
  SinCos4_SSE(v, sinV, cosV);

  __m128 vCosU = _mm_load_ps(cosU);
  __m128 vSinU = _mm_load_ps(sinU);
  __m128 vCosV = _mm_load_ps(cosV);
  __m128 vSinV = _mm_load_ps(sinV);

  __m128 vRadius = _mm_set1_ps(params.Radius);
  __m128 vR1 = _mm_mul_ps(vRadius, vCosV);
  __m128 vR2 = _mm_mul_ps(vRadius, vSinV);

  __m128 vA1 = _mm_mul_ps(vR1, vCosU);
  __m128 vA2 = _mm_mul_ps(vR1, vSinU);
  __m128 vA3 = _mm_mul_ps(vR2, vCosU);
  __m128 vA4 = _mm_mul_ps(vR2, vSinU);

  __m128 vXDirX = _mm_set1_ps(params.XDirX);
  __m128 vXDirY = _mm_set1_ps(params.XDirY);
  __m128 vXDirZ = _mm_set1_ps(params.XDirZ);
  __m128 vYDirX = _mm_set1_ps(params.YDirX);
  __m128 vYDirY = _mm_set1_ps(params.YDirY);
  __m128 vYDirZ = _mm_set1_ps(params.YDirZ);
  __m128 vZDirX = _mm_set1_ps(params.ZDirX);
  __m128 vZDirY = _mm_set1_ps(params.ZDirY);
  __m128 vZDirZ = _mm_set1_ps(params.ZDirZ);
  __m128 vLocX = _mm_set1_ps(params.LocX);
  __m128 vLocY = _mm_set1_ps(params.LocY);
  __m128 vLocZ = _mm_set1_ps(params.LocZ);

  // P = Loc + A1*XDir + A2*YDir + R2*ZDir
  _mm_store_ps(outPx, _mm_add_ps(vLocX, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirX), _mm_mul_ps(vA2, vYDirX)), _mm_mul_ps(vR2, vZDirX))));
  _mm_store_ps(outPy, _mm_add_ps(vLocY, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirY), _mm_mul_ps(vA2, vYDirY)), _mm_mul_ps(vR2, vZDirY))));
  _mm_store_ps(outPz, _mm_add_ps(vLocZ, _mm_add_ps(_mm_add_ps(_mm_mul_ps(vA1, vXDirZ), _mm_mul_ps(vA2, vYDirZ)), _mm_mul_ps(vR2, vZDirZ))));

  // Vu = -A2*XDir + A1*YDir
  __m128 vNeg = _mm_set1_ps(-1.0f);
  _mm_store_ps(outDuX, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirX), _mm_mul_ps(vA1, vYDirX)));
  _mm_store_ps(outDuY, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirY), _mm_mul_ps(vA1, vYDirY)));
  _mm_store_ps(outDuZ, _mm_add_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA2), vXDirZ), _mm_mul_ps(vA1, vYDirZ)));

  // Vv = -A3*XDir - A4*YDir + R1*ZDir
  _mm_store_ps(outDvX, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirX), _mm_mul_ps(vA4, vYDirX)), _mm_mul_ps(vR1, vZDirX)));
  _mm_store_ps(outDvY, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirY), _mm_mul_ps(vA4, vYDirY)), _mm_mul_ps(vR1, vZDirY)));
  _mm_store_ps(outDvZ, _mm_add_ps(_mm_sub_ps(_mm_mul_ps(_mm_mul_ps(vNeg, vA3), vXDirZ), _mm_mul_ps(vA4, vYDirZ)), _mm_mul_ps(vR1, vZDirZ)));
}

#endif // SIMD_SURFACE_SSE

//=============================================================================
// Dispatcher: automatically selects the best SIMD implementation
//=============================================================================

//! Evaluate D1 for 4 (u,v) points on an elementary surface
//! Uses SIMD if available, otherwise falls back to scalar
inline void SurfaceD1_4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  switch (params.Type)
  {
    case GeomAbs_Torus:
#ifdef SIMD_SURFACE_NEON
      TorusD1_NEON4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#elif defined(SIMD_SURFACE_SSE)
      TorusD1_SSE4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#else
      // Scalar fallback
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float cosV = std::cos(v[i]), sinV = std::sin(v[i]);
        float R1 = params.MinorRadius * cosV;
        float R2 = params.MinorRadius * sinV;
        float R = params.MajorRadius + R1;
        float A1 = R * cosU, A2 = R * sinU;
        float A3 = R2 * cosU, A4 = R2 * sinU;
        outPx[i] = params.LocX + A1*params.XDirX + A2*params.YDirX + R2*params.ZDirX;
        outPy[i] = params.LocY + A1*params.XDirY + A2*params.YDirY + R2*params.ZDirY;
        outPz[i] = params.LocZ + A1*params.XDirZ + A2*params.YDirZ + R2*params.ZDirZ;
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        outDvX[i] = -A3*params.XDirX - A4*params.YDirX + R1*params.ZDirX;
        outDvY[i] = -A3*params.XDirY - A4*params.YDirY + R1*params.ZDirY;
        outDvZ[i] = -A3*params.XDirZ - A4*params.YDirZ + R1*params.ZDirZ;
      }
#endif
      break;

    case GeomAbs_Cylinder:
#ifdef SIMD_SURFACE_NEON
      CylinderD1_NEON4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#elif defined(SIMD_SURFACE_SSE)
      CylinderD1_SSE4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#else
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float A1 = params.Radius * cosU, A2 = params.Radius * sinU;
        outPx[i] = params.LocX + A1*params.XDirX + A2*params.YDirX + v[i]*params.ZDirX;
        outPy[i] = params.LocY + A1*params.XDirY + A2*params.YDirY + v[i]*params.ZDirY;
        outPz[i] = params.LocZ + A1*params.XDirZ + A2*params.YDirZ + v[i]*params.ZDirZ;
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        outDvX[i] = params.ZDirX;
        outDvY[i] = params.ZDirY;
        outDvZ[i] = params.ZDirZ;
      }
#endif
      break;

    case GeomAbs_Sphere:
#ifdef SIMD_SURFACE_NEON
      SphereD1_NEON4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#elif defined(SIMD_SURFACE_SSE)
      SphereD1_SSE4(params, u, v, outPx, outPy, outPz, outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ);
#else
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float cosV = std::cos(v[i]), sinV = std::sin(v[i]);
        float R1 = params.Radius * cosV, R2 = params.Radius * sinV;
        float A1 = R1 * cosU, A2 = R1 * sinU;
        float A3 = R2 * cosU, A4 = R2 * sinU;
        outPx[i] = params.LocX + A1*params.XDirX + A2*params.YDirX + R2*params.ZDirX;
        outPy[i] = params.LocY + A1*params.XDirY + A2*params.YDirY + R2*params.ZDirY;
        outPz[i] = params.LocZ + A1*params.XDirZ + A2*params.YDirZ + R2*params.ZDirZ;
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        outDvX[i] = -A3*params.XDirX - A4*params.YDirX + R1*params.ZDirX;
        outDvY[i] = -A3*params.XDirY - A4*params.YDirY + R1*params.ZDirY;
        outDvZ[i] = -A3*params.XDirZ - A4*params.YDirZ + R1*params.ZDirZ;
      }
#endif
      break;

    default:
      // Unsupported surface type - leave outputs unchanged
      break;
  }
}

//! Evaluate D2 for 4 (u,v) points on an elementary surface
//! Uses SIMD if available, otherwise falls back to scalar
inline void SurfaceD2_4(
  const SIMDSurfaceParams& params,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ,
  float* outDuuX, float* outDuuY, float* outDuuZ,
  float* outDvvX, float* outDvvY, float* outDvvZ,
  float* outDuvX, float* outDuvY, float* outDuvZ)
{
  switch (params.Type)
  {
    case GeomAbs_Torus:
#ifdef SIMD_SURFACE_NEON
      TorusD2_NEON4(params, u, v, outPx, outPy, outPz,
                    outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ,
                    outDuuX, outDuuY, outDuuZ, outDvvX, outDvvY, outDvvZ,
                    outDuvX, outDuvY, outDuvZ);
#else
      // Scalar fallback for torus D2
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float cosV = std::cos(v[i]), sinV = std::sin(v[i]);
        float R1 = params.MinorRadius * cosV;
        float R2 = params.MinorRadius * sinV;
        float R = params.MajorRadius + R1;
        float A1 = R * cosU, A2 = R * sinU;
        float A3 = R2 * cosU, A4 = R2 * sinU;
        float A5 = R1 * cosU, A6 = R1 * sinU;
        // Som1 = A1*XDir + A2*YDir
        float Som1X = A1*params.XDirX + A2*params.YDirX;
        float Som1Y = A1*params.XDirY + A2*params.YDirY;
        float Som1Z = A1*params.XDirZ + A2*params.YDirZ;
        float R2ZX = R2*params.ZDirX, R2ZY = R2*params.ZDirY, R2ZZ = R2*params.ZDirZ;
        // P
        outPx[i] = params.LocX + Som1X + R2ZX;
        outPy[i] = params.LocY + Som1Y + R2ZY;
        outPz[i] = params.LocZ + Som1Z + R2ZZ;
        // Du
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        // Dv
        outDvX[i] = -A3*params.XDirX - A4*params.YDirX + R1*params.ZDirX;
        outDvY[i] = -A3*params.XDirY - A4*params.YDirY + R1*params.ZDirY;
        outDvZ[i] = -A3*params.XDirZ - A4*params.YDirZ + R1*params.ZDirZ;
        // Duu = -Som1
        outDuuX[i] = -Som1X; outDuuY[i] = -Som1Y; outDuuZ[i] = -Som1Z;
        // Dvv
        outDvvX[i] = -A5*params.XDirX - A6*params.YDirX - R2ZX;
        outDvvY[i] = -A5*params.XDirY - A6*params.YDirY - R2ZY;
        outDvvZ[i] = -A5*params.XDirZ - A6*params.YDirZ - R2ZZ;
        // Duv
        outDuvX[i] = A4*params.XDirX - A3*params.YDirX;
        outDuvY[i] = A4*params.XDirY - A3*params.YDirY;
        outDuvZ[i] = A4*params.XDirZ - A3*params.YDirZ;
      }
#endif
      break;

    case GeomAbs_Cylinder:
#ifdef SIMD_SURFACE_NEON
      CylinderD2_NEON4(params, u, v, outPx, outPy, outPz,
                       outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ,
                       outDuuX, outDuuY, outDuuZ, outDvvX, outDvvY, outDvvZ,
                       outDuvX, outDuvY, outDuvZ);
#else
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float A1 = params.Radius * cosU, A2 = params.Radius * sinU;
        float Som1X = A1*params.XDirX + A2*params.YDirX;
        float Som1Y = A1*params.XDirY + A2*params.YDirY;
        float Som1Z = A1*params.XDirZ + A2*params.YDirZ;
        outPx[i] = params.LocX + Som1X + v[i]*params.ZDirX;
        outPy[i] = params.LocY + Som1Y + v[i]*params.ZDirY;
        outPz[i] = params.LocZ + Som1Z + v[i]*params.ZDirZ;
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        outDvX[i] = params.ZDirX; outDvY[i] = params.ZDirY; outDvZ[i] = params.ZDirZ;
        outDuuX[i] = -Som1X; outDuuY[i] = -Som1Y; outDuuZ[i] = -Som1Z;
        outDvvX[i] = 0; outDvvY[i] = 0; outDvvZ[i] = 0;
        outDuvX[i] = 0; outDuvY[i] = 0; outDuvZ[i] = 0;
      }
#endif
      break;

    case GeomAbs_Sphere:
#ifdef SIMD_SURFACE_NEON
      SphereD2_NEON4(params, u, v, outPx, outPy, outPz,
                     outDuX, outDuY, outDuZ, outDvX, outDvY, outDvZ,
                     outDuuX, outDuuY, outDuuZ, outDvvX, outDvvY, outDvvZ,
                     outDuvX, outDuvY, outDuvZ);
#else
      for (int i = 0; i < 4; ++i) {
        float cosU = std::cos(u[i]), sinU = std::sin(u[i]);
        float cosV = std::cos(v[i]), sinV = std::sin(v[i]);
        float R1 = params.Radius * cosV, R2 = params.Radius * sinV;
        float A1 = R1 * cosU, A2 = R1 * sinU;
        float A3 = R2 * cosU, A4 = R2 * sinU;
        float Som1X = A1*params.XDirX + A2*params.YDirX;
        float Som1Y = A1*params.XDirY + A2*params.YDirY;
        float Som1Z = A1*params.XDirZ + A2*params.YDirZ;
        float R2ZX = R2*params.ZDirX, R2ZY = R2*params.ZDirY, R2ZZ = R2*params.ZDirZ;
        outPx[i] = params.LocX + Som1X + R2ZX;
        outPy[i] = params.LocY + Som1Y + R2ZY;
        outPz[i] = params.LocZ + Som1Z + R2ZZ;
        outDuX[i] = -A2*params.XDirX + A1*params.YDirX;
        outDuY[i] = -A2*params.XDirY + A1*params.YDirY;
        outDuZ[i] = -A2*params.XDirZ + A1*params.YDirZ;
        outDvX[i] = -A3*params.XDirX - A4*params.YDirX + R1*params.ZDirX;
        outDvY[i] = -A3*params.XDirY - A4*params.YDirY + R1*params.ZDirY;
        outDvZ[i] = -A3*params.XDirZ - A4*params.YDirZ + R1*params.ZDirZ;
        outDuuX[i] = -Som1X; outDuuY[i] = -Som1Y; outDuuZ[i] = -Som1Z;
        outDvvX[i] = -Som1X - R2ZX; outDvvY[i] = -Som1Y - R2ZY; outDvvZ[i] = -Som1Z - R2ZZ;
        outDuvX[i] = A4*params.XDirX - A3*params.YDirX;
        outDuvY[i] = A4*params.XDirY - A3*params.YDirY;
        outDuvZ[i] = A4*params.XDirZ - A3*params.YDirZ;
      }
#endif
      break;

    default:
      break;
  }
}

//! Check if a surface type supports SIMD D1/D2 evaluation
inline bool SupportsSIMDSurface(GeomAbs_SurfaceType theType)
{
  return theType == GeomAbs_Torus ||
         theType == GeomAbs_Cylinder ||
         theType == GeomAbs_Sphere ||
         theType == GeomAbs_Cone;
}

//=============================================================================
// ANALYTIC RAY-SURFACE INTERSECTION (Skip Newton entirely!)
//=============================================================================

//! Analytic ray-sphere intersection
//! Solves: |O + t*D - C|² = R²  →  quadratic in t
//! Returns true if hit found, outputs t, u, v
//! @param params  Surface parameters (must be GeomAbs_Sphere)
//! @param rayOrgX,Y,Z  Ray origin
//! @param rayDirX,Y,Z  Ray direction (normalized)
//! @param tMin, tMax   Ray interval
//! @param outT         Output: hit parameter t
//! @param outU, outV   Output: surface parameters
inline bool AnalyticSphereIntersect(
  const SIMDSurfaceParams& params,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float tMin, float tMax,
  float& outT, float& outU, float& outV)
{
  // Transform ray to local coordinates
  float locOrgX = rayOrgX - params.LocX;
  float locOrgY = rayOrgY - params.LocY;
  float locOrgZ = rayOrgZ - params.LocZ;

  // Project to local frame: O_local = (O·X, O·Y, O·Z)
  float ox = locOrgX * params.XDirX + locOrgY * params.XDirY + locOrgZ * params.XDirZ;
  float oy = locOrgX * params.YDirX + locOrgY * params.YDirY + locOrgZ * params.YDirZ;
  float oz = locOrgX * params.ZDirX + locOrgY * params.ZDirY + locOrgZ * params.ZDirZ;

  float dx = rayDirX * params.XDirX + rayDirY * params.XDirY + rayDirZ * params.XDirZ;
  float dy = rayDirX * params.YDirX + rayDirY * params.YDirY + rayDirZ * params.YDirZ;
  float dz = rayDirX * params.ZDirX + rayDirY * params.ZDirY + rayDirZ * params.ZDirZ;

  // Sphere equation in local coords: x² + y² + z² = R²
  // Substitute ray: (ox + t*dx)² + (oy + t*dy)² + (oz + t*dz)² = R²
  // Quadratic: a*t² + b*t + c = 0
  float R = params.Radius;
  float a = dx*dx + dy*dy + dz*dz;  // Should be 1 for normalized direction
  float b = 2.0f * (ox*dx + oy*dy + oz*dz);
  float c = ox*ox + oy*oy + oz*oz - R*R;

  float discriminant = b*b - 4.0f*a*c;
  if (discriminant < 0.0f)
    return false;

  float sqrtDisc = std::sqrt(discriminant);
  float t1 = (-b - sqrtDisc) / (2.0f * a);
  float t2 = (-b + sqrtDisc) / (2.0f * a);

  // Pick closest valid t
  float t = t1;
  if (t < tMin || t > tMax)
  {
    t = t2;
    if (t < tMin || t > tMax)
      return false;
  }

  // Compute hit point in local coordinates
  float hx = ox + t * dx;
  float hy = oy + t * dy;
  float hz = oz + t * dz;

  // Compute (u, v) from hit point
  // Sphere parametrization: S(u,v) = (R*cos(v)*cos(u), R*cos(v)*sin(u), R*sin(v))
  // v = asin(z/R), u = atan2(y, x)
  outT = t;
  outV = std::asin(std::clamp(hz / R, -1.0f, 1.0f));
  outU = std::atan2(hy, hx);
  if (outU < 0.0f) outU += 2.0f * 3.14159265358979323846f;

  return true;
}

//! Analytic ray-cylinder intersection (infinite cylinder)
//! Solves: (O + t*D - C)² projected onto XY plane = R²
//! Returns true if hit found
inline bool AnalyticCylinderIntersect(
  const SIMDSurfaceParams& params,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float tMin, float tMax,
  float vMin, float vMax,  // V bounds for finite cylinder
  float& outT, float& outU, float& outV)
{
  // Transform ray to local coordinates
  float locOrgX = rayOrgX - params.LocX;
  float locOrgY = rayOrgY - params.LocY;
  float locOrgZ = rayOrgZ - params.LocZ;

  float ox = locOrgX * params.XDirX + locOrgY * params.XDirY + locOrgZ * params.XDirZ;
  float oy = locOrgX * params.YDirX + locOrgY * params.YDirY + locOrgZ * params.YDirZ;
  float oz = locOrgX * params.ZDirX + locOrgY * params.ZDirY + locOrgZ * params.ZDirZ;

  float dx = rayDirX * params.XDirX + rayDirY * params.XDirY + rayDirZ * params.XDirZ;
  float dy = rayDirX * params.YDirX + rayDirY * params.YDirY + rayDirZ * params.YDirZ;
  float dz = rayDirX * params.ZDirX + rayDirY * params.ZDirY + rayDirZ * params.ZDirZ;

  // Cylinder equation in local coords: x² + y² = R² (z is axis)
  // Substitute ray: (ox + t*dx)² + (oy + t*dy)² = R²
  float R = params.Radius;
  float a = dx*dx + dy*dy;
  float b = 2.0f * (ox*dx + oy*dy);
  float c = ox*ox + oy*oy - R*R;

  if (std::abs(a) < 1e-10f)
    return false;  // Ray parallel to cylinder axis

  float discriminant = b*b - 4.0f*a*c;
  if (discriminant < 0.0f)
    return false;

  float sqrtDisc = std::sqrt(discriminant);
  float t1 = (-b - sqrtDisc) / (2.0f * a);
  float t2 = (-b + sqrtDisc) / (2.0f * a);

  // Try both roots, check V bounds
  for (float t : {t1, t2})
  {
    if (t < tMin || t > tMax)
      continue;

    float hx = ox + t * dx;
    float hy = oy + t * dy;
    float hz = oz + t * dz;  // This is V

    // Check V bounds
    if (hz < vMin || hz > vMax)
      continue;

    // Compute U = atan2(y, x)
    float u = std::atan2(hy, hx);
    if (u < 0.0f) u += 2.0f * 3.14159265358979323846f;

    outT = t;
    outU = u;
    outV = hz;
    return true;
  }

  return false;
}

//! Analytic ray-cone intersection
//! Cone equation in local coords: x² + y² = (tan(α) * z)² where α is semi-angle
//! Solves quadratic in t
//! Returns true if hit found
inline bool AnalyticConeIntersect(
  const SIMDSurfaceParams& params,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float tMin, float tMax,
  float vMin, float vMax,  // V bounds for finite cone
  float& outT, float& outU, float& outV)
{
  // Transform ray to local coordinates
  float locOrgX = rayOrgX - params.LocX;
  float locOrgY = rayOrgY - params.LocY;
  float locOrgZ = rayOrgZ - params.LocZ;

  float ox = locOrgX * params.XDirX + locOrgY * params.XDirY + locOrgZ * params.XDirZ;
  float oy = locOrgX * params.YDirX + locOrgY * params.YDirY + locOrgZ * params.YDirZ;
  float oz = locOrgX * params.ZDirX + locOrgY * params.ZDirY + locOrgZ * params.ZDirZ;

  float dx = rayDirX * params.XDirX + rayDirY * params.XDirY + rayDirZ * params.XDirZ;
  float dy = rayDirX * params.YDirX + rayDirY * params.YDirY + rayDirZ * params.YDirZ;
  float dz = rayDirX * params.ZDirX + rayDirY * params.ZDirY + rayDirZ * params.ZDirZ;

  // Cone equation: x² + y² = (tan(α) * z)²
  // Let k = tan(semi-angle), so x² + y² - k²z² = 0
  float tanAngle = std::tan(params.SemiAngle);
  float k2 = tanAngle * tanAngle;

  // Substitute ray: (ox + t*dx)² + (oy + t*dy)² - k²(oz + t*dz)² = 0
  // Expand: (dx² + dy² - k²dz²)t² + 2(ox*dx + oy*dy - k²oz*dz)t + (ox² + oy² - k²oz²) = 0
  float a = dx*dx + dy*dy - k2*dz*dz;
  float b = 2.0f * (ox*dx + oy*dy - k2*oz*dz);
  float c = ox*ox + oy*oy - k2*oz*oz;

  float discriminant = b*b - 4.0f*a*c;

  // Handle degenerate case (ray parallel to cone surface)
  if (std::abs(a) < 1e-10f)
  {
    // Linear equation: bt + c = 0
    if (std::abs(b) < 1e-10f)
      return false;
    float t = -c / b;
    if (t < tMin || t > tMax)
      return false;

    float hz = oz + t * dz;
    if (hz < vMin || hz > vMax)
      return false;

    float hx = ox + t * dx;
    float hy = oy + t * dy;
    outT = t;
    outU = std::atan2(hy, hx);
    if (outU < 0.0f) outU += 2.0f * 3.14159265358979323846f;
    outV = hz;
    return true;
  }

  if (discriminant < 0.0f)
    return false;

  float sqrtDisc = std::sqrt(discriminant);
  float t1 = (-b - sqrtDisc) / (2.0f * a);
  float t2 = (-b + sqrtDisc) / (2.0f * a);

  // Try both roots, check V bounds and pick closest valid
  for (float t : {t1, t2})
  {
    if (t < tMin || t > tMax)
      continue;

    float hx = ox + t * dx;
    float hy = oy + t * dy;
    float hz = oz + t * dz;  // This is V

    // Check V bounds (finite cone)
    if (hz < vMin || hz > vMax)
      continue;

    // Compute U = atan2(y, x)
    float u = std::atan2(hy, hx);
    if (u < 0.0f) u += 2.0f * 3.14159265358979323846f;

    outT = t;
    outU = u;
    outV = hz;
    return true;
  }

  return false;
}

//! Analytic ray-plane intersection
//! Plane equation: (P - Loc) · Normal = 0, where Normal = ZDir
//! Returns true if hit found
inline bool AnalyticPlaneIntersect(
  const SIMDSurfaceParams& params,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float tMin, float tMax,
  float& outT, float& outU, float& outV)
{
  // Plane normal is ZDir, plane passes through Loc
  // Plane equation: (P - Loc) · ZDir = 0
  // Ray: P = O + t*D
  // Substitute: (O + t*D - Loc) · ZDir = 0
  // t = (Loc - O) · ZDir / (D · ZDir)

  float denom = rayDirX * params.ZDirX + rayDirY * params.ZDirY + rayDirZ * params.ZDirZ;

  if (std::abs(denom) < 1e-10f)
    return false;  // Ray parallel to plane

  float locToOrgX = params.LocX - rayOrgX;
  float locToOrgY = params.LocY - rayOrgY;
  float locToOrgZ = params.LocZ - rayOrgZ;

  float numer = locToOrgX * params.ZDirX + locToOrgY * params.ZDirY + locToOrgZ * params.ZDirZ;
  float t = numer / denom;

  if (t < tMin || t > tMax)
    return false;

  // Compute hit point
  float hx = rayOrgX + t * rayDirX;
  float hy = rayOrgY + t * rayDirY;
  float hz = rayOrgZ + t * rayDirZ;

  // Transform to local coordinates for (u, v)
  float localX = (hx - params.LocX) * params.XDirX + (hy - params.LocY) * params.XDirY + (hz - params.LocZ) * params.XDirZ;
  float localY = (hx - params.LocX) * params.YDirX + (hy - params.LocY) * params.YDirY + (hz - params.LocZ) * params.YDirZ;

  outT = t;
  outU = localX;  // U is projection onto XDir
  outV = localY;  // V is projection onto YDir

  return true;
}

//! Solve cubic equation x³ + px + q = 0 using Cardano's formula
//! Returns number of real roots (1 or 3), fills roots array
inline int SolveCubicDepressed(float p, float q, float roots[3])
{
  const float eps = 1e-9f;

  // Discriminant: Δ = -4p³ - 27q²
  float p3 = p * p * p;
  float q2 = q * q;
  float discriminant = -4.0f * p3 - 27.0f * q2;

  if (discriminant > eps)
  {
    // Three distinct real roots - use trigonometric method
    float m = 2.0f * std::sqrt(-p / 3.0f);
    float theta = std::acos(3.0f * q / (p * m)) / 3.0f;
    roots[0] = m * std::cos(theta);
    roots[1] = m * std::cos(theta - 2.0943951023931953f);  // 2π/3
    roots[2] = m * std::cos(theta - 4.1887902047863905f);  // 4π/3
    return 3;
  }
  else if (discriminant < -eps)
  {
    // One real root - use Cardano's formula
    float sqrtQ = std::sqrt(q2 / 4.0f + p3 / 27.0f);
    float A = std::cbrt(-q / 2.0f + sqrtQ);
    float B = std::cbrt(-q / 2.0f - sqrtQ);
    roots[0] = A + B;
    return 1;
  }
  else
  {
    // Discriminant ≈ 0: repeated roots
    if (std::abs(q) < eps)
    {
      roots[0] = 0.0f;
      return 1;
    }
    float u = 3.0f * q / p;
    roots[0] = u;
    roots[1] = -u / 2.0f;
    return 2;
  }
}

//! Solve quartic equation t⁴ + at³ + bt² + ct + d = 0 using Ferrari's method
//! Returns number of real roots (0-4), fills roots array
inline int SolveQuarticFerrari(float a, float b, float c, float d, float roots[4])
{
  const float eps = 1e-9f;

  // Convert to depressed quartic: y⁴ + py² + qy + r = 0
  // where t = y - a/4
  float a2 = a * a;
  float a3 = a2 * a;
  float a4 = a2 * a2;

  float p = b - 0.375f * a2;
  float q = c - 0.5f * a * b + 0.125f * a3;
  float r = d - 0.25f * a * c + 0.0625f * a2 * b - 0.01171875f * a4;

  float shift = -0.25f * a;

  // Special case: biquadratic (q = 0)
  if (std::abs(q) < eps)
  {
    // y⁴ + py² + r = 0, let z = y²
    float disc = p * p - 4.0f * r;
    if (disc < -eps) return 0;

    disc = std::max(0.0f, disc);
    float sqrtDisc = std::sqrt(disc);
    float z1 = (-p + sqrtDisc) / 2.0f;
    float z2 = (-p - sqrtDisc) / 2.0f;

    int numRoots = 0;
    if (z1 >= -eps)
    {
      float y = std::sqrt(std::max(0.0f, z1));
      roots[numRoots++] = y + shift;
      roots[numRoots++] = -y + shift;
    }
    if (z2 >= -eps && std::abs(z2 - z1) > eps)
    {
      float y = std::sqrt(std::max(0.0f, z2));
      roots[numRoots++] = y + shift;
      roots[numRoots++] = -y + shift;
    }
    return numRoots;
  }

  // Ferrari's method: find auxiliary cubic root
  // Resolvent cubic: m³ + (5p/2)m² + (2p² - r)m + (p³/2 - pr/2 - q²/8) = 0
  // Substitute m = z - 5p/6 to get depressed cubic z³ + Pz + Q = 0

  float p2 = p * p;
  float cubicA = 2.5f * p;
  float cubicB = 2.0f * p2 - r;
  float cubicC = 0.5f * p * p2 - 0.5f * p * r - 0.125f * q * q;

  // Convert cubic m³ + Am² + Bm + C = 0 to depressed form z³ + Pz + Q = 0
  float cubicShift = -cubicA / 3.0f;
  float P = cubicB - cubicA * cubicA / 3.0f;
  float Q = cubicC - cubicA * cubicB / 3.0f + 2.0f * cubicA * cubicA * cubicA / 27.0f;

  float cubicRoots[3];
  int numCubicRoots = SolveCubicDepressed(P, Q, cubicRoots);

  // Find largest real root of resolvent cubic
  float m = cubicRoots[0] + cubicShift;
  for (int i = 1; i < numCubicRoots; ++i)
  {
    float mi = cubicRoots[i] + cubicShift;
    if (mi > m) m = mi;
  }

  // Now factor depressed quartic using m:
  // y⁴ + py² + qy + r = (y² + m/2 + sqrt(2m + p) * y)(y² + m/2 - sqrt(2m + p) * y + ...)

  float sqrtTerm = 2.0f * m + p;
  if (sqrtTerm < -eps) return 0;  // No real factorization
  sqrtTerm = std::sqrt(std::max(0.0f, sqrtTerm));

  // Solve two quadratics:
  // y² + sqrtTerm * y + (m/2 + (m*p - q)/(2*sqrtTerm)) = 0
  // y² - sqrtTerm * y + (m/2 + (m*p + q)/(2*sqrtTerm)) = 0

  int numRoots = 0;

  if (std::abs(sqrtTerm) > eps)
  {
    float halfM = m / 2.0f;
    float term = (m * p - q) / (2.0f * sqrtTerm);

    // First quadratic: y² + sqrtTerm*y + (halfM + term) = 0
    float disc1 = sqrtTerm * sqrtTerm - 4.0f * (halfM + term);
    if (disc1 >= -eps)
    {
      disc1 = std::sqrt(std::max(0.0f, disc1));
      roots[numRoots++] = (-sqrtTerm + disc1) / 2.0f + shift;
      roots[numRoots++] = (-sqrtTerm - disc1) / 2.0f + shift;
    }

    term = (m * p + q) / (2.0f * sqrtTerm);

    // Second quadratic: y² - sqrtTerm*y + (halfM + term) = 0
    float disc2 = sqrtTerm * sqrtTerm - 4.0f * (halfM + term);
    if (disc2 >= -eps)
    {
      disc2 = std::sqrt(std::max(0.0f, disc2));
      roots[numRoots++] = (sqrtTerm + disc2) / 2.0f + shift;
      roots[numRoots++] = (sqrtTerm - disc2) / 2.0f + shift;
    }
  }
  else
  {
    // sqrtTerm ≈ 0, degenerate case
    float disc = m * m / 4.0f - r;
    if (disc >= -eps)
    {
      disc = std::sqrt(std::max(0.0f, disc));
      float y2_1 = -m / 2.0f + disc;
      float y2_2 = -m / 2.0f - disc;

      if (y2_1 >= -eps)
      {
        float y = std::sqrt(std::max(0.0f, y2_1));
        roots[numRoots++] = y + shift;
        roots[numRoots++] = -y + shift;
      }
      if (y2_2 >= -eps)
      {
        float y = std::sqrt(std::max(0.0f, y2_2));
        roots[numRoots++] = y + shift;
        roots[numRoots++] = -y + shift;
      }
    }
  }

  return numRoots;
}

//! Analytic ray-torus intersection
//! Solves quartic equation using Ferrari's method (NO NEWTON ITERATIONS!)
//! Torus: ((x² + y² + z² + R² - r²)² = 4R²(x² + y²)
//! Returns true if hit found
inline bool AnalyticTorusIntersect(
  const SIMDSurfaceParams& params,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float tMin, float tMax,
  float& outT, float& outU, float& outV)
{
  // Transform ray to local coordinates
  float locOrgX = rayOrgX - params.LocX;
  float locOrgY = rayOrgY - params.LocY;
  float locOrgZ = rayOrgZ - params.LocZ;

  float ox = locOrgX * params.XDirX + locOrgY * params.XDirY + locOrgZ * params.XDirZ;
  float oy = locOrgX * params.YDirX + locOrgY * params.YDirY + locOrgZ * params.YDirZ;
  float oz = locOrgX * params.ZDirX + locOrgY * params.ZDirY + locOrgZ * params.ZDirZ;

  float dx = rayDirX * params.XDirX + rayDirY * params.XDirY + rayDirZ * params.XDirZ;
  float dy = rayDirX * params.YDirX + rayDirY * params.YDirY + rayDirZ * params.YDirZ;
  float dz = rayDirX * params.ZDirX + rayDirY * params.ZDirY + rayDirZ * params.ZDirZ;

  float R = params.MajorRadius;  // Major radius
  float r = params.MinorRadius;  // Minor radius

  // Precompute commonly used values
  float R2 = R * R;
  float r2 = r * r;
  float ox2 = ox * ox, oy2 = oy * oy, oz2 = oz * oz;
  float dx2 = dx * dx, dy2 = dy * dy, dz2 = dz * dz;
  float oxdx = ox * dx, oydy = oy * dy, ozdz = oz * dz;

  // The torus implicit equation:
  // (x² + y² + z² + R² - r²)² = 4R²(x² + y²)
  //
  // Substituting ray x = ox + t*dx, etc. gives quartic:
  // c4*t⁴ + c3*t³ + c2*t² + c1*t + c0 = 0

  float sumD2 = dx2 + dy2 + dz2;  // |D|² (should be ~1)
  float sumOD = oxdx + oydy + ozdz;  // O·D
  float sumO2 = ox2 + oy2 + oz2;  // |O|²

  float K = sumO2 + R2 - r2;

  float c4 = sumD2 * sumD2;
  float c3 = 4.0f * sumD2 * sumOD;
  float c2 = 2.0f * sumD2 * K + 4.0f * sumOD * sumOD - 4.0f * R2 * (dx2 + dy2);
  float c1 = 4.0f * K * sumOD - 8.0f * R2 * (oxdx + oydy);
  float c0 = K * K - 4.0f * R2 * (ox2 + oy2);

  // Normalize to monic quartic: t⁴ + at³ + bt² + ct + d = 0
  if (std::abs(c4) < 1e-12f)
    return false;

  float invC4 = 1.0f / c4;
  float a = c3 * invC4;
  float b = c2 * invC4;
  float c = c1 * invC4;
  float d = c0 * invC4;

  // Solve quartic using Ferrari's method
  float roots[4];
  int numRoots = SolveQuarticFerrari(a, b, c, d, roots);

  if (numRoots == 0)
    return false;

  // Find smallest positive root in [tMin, tMax]
  float bestT = tMax + 1.0f;
  for (int i = 0; i < numRoots; ++i)
  {
    float t = roots[i];
    if (t >= tMin && t <= tMax && t < bestT)
      bestT = t;
  }

  if (bestT > tMax)
    return false;

  outT = bestT;

  // Compute (u, v) from hit point
  float hx = ox + bestT * dx;
  float hy = oy + bestT * dy;
  float hz = oz + bestT * dz;

  // Torus parametrization:
  // S(u,v) = ((R + r*cos(v))*cos(u), (R + r*cos(v))*sin(u), r*sin(v))
  // u = atan2(y, x)
  // v = atan2(z, sqrt(x² + y²) - R)
  outU = std::atan2(hy, hx);
  if (outU < 0.0f) outU += 2.0f * 3.14159265358979323846f;

  float rxy = std::sqrt(hx*hx + hy*hy);
  outV = std::atan2(hz, rxy - R);
  if (outV < 0.0f) outV += 2.0f * 3.14159265358979323846f;

  return true;
}

//! Check if a surface supports analytic intersection (skip Newton)
inline bool SupportsAnalyticIntersect(GeomAbs_SurfaceType theType)
{
  return theType == GeomAbs_Plane ||
         theType == GeomAbs_Sphere ||
         theType == GeomAbs_Cylinder ||
         theType == GeomAbs_Cone ||
         theType == GeomAbs_Torus;
}

//=============================================================================
// Bézier Clipping for Direct Ray-Patch Intersection
// Based on Nishita, Sederberg & Kakimoto (1990) with cubic convergence
//=============================================================================

//! Maximum degree for Bézier patches (bicubic = 3x3)
constexpr int BEZIER_MAX_DEGREE = 7;

//! Bézier clipping result
struct BezierClipResult
{
  bool   Found;      //!< True if intersection found
  float  U, V;       //!< Parameter values on patch
  float  T;          //!< Ray parameter (distance)
  float  Px, Py, Pz; //!< Intersection point
};

//! 2D point for projected control polygon
struct Point2D
{
  float x, y;
  Point2D() : x(0), y(0) {}
  Point2D(float ax, float ay) : x(ax), y(ay) {}
};

//! Find min/max of a 1D Bézier curve's control polygon (for clipping bounds)
//! Uses convex hull property: curve lies within control polygon bounds
inline void BezierBounds1D(const float* ctrl, int n, float& minVal, float& maxVal)
{
  minVal = ctrl[0];
  maxVal = ctrl[0];
  for (int i = 1; i <= n; ++i)
  {
    minVal = std::min(minVal, ctrl[i]);
    maxVal = std::max(maxVal, ctrl[i]);
  }
}

//! de Casteljau subdivision of 1D Bézier curve at parameter t
//! Splits curve into [0,t] and [t,1] portions
//! left[0..n] = control points for [0,t]
//! right[0..n] = control points for [t,1]
inline void DeCasteljau1D(const float* ctrl, int n, float t, float* left, float* right)
{
  // Work array
  float work[BEZIER_MAX_DEGREE + 1];
  for (int i = 0; i <= n; ++i)
    work[i] = ctrl[i];

  left[0] = work[0];
  right[n] = work[n];

  for (int j = 1; j <= n; ++j)
  {
    for (int i = 0; i <= n - j; ++i)
    {
      work[i] = (1.0f - t) * work[i] + t * work[i + 1];
    }
    left[j] = work[0];
    right[n - j] = work[n - j];
  }
}

//! Clip a 1D Bézier curve to find parameter range where it could cross zero
//! Returns [tMin, tMax] range, or tMin > tMax if no crossing possible
//! Uses fat line clipping: finds where control polygon crosses the axis
inline void ClipBezier1D(const float* ctrl, int n, float& tMin, float& tMax)
{
  // Check if all control points have same sign (no crossing)
  bool allPos = true, allNeg = true;
  for (int i = 0; i <= n; ++i)
  {
    if (ctrl[i] < 0) allPos = false;
    if (ctrl[i] > 0) allNeg = false;
  }

  if (allPos || allNeg)
  {
    tMin = 1.0f;
    tMax = 0.0f; // Invalid range = no crossing
    return;
  }

  // Find convex hull crossings with y=0 axis
  // For simplicity, use linear interpolation between consecutive control points
  tMin = 1.0f;
  tMax = 0.0f;

  for (int i = 0; i < n; ++i)
  {
    float y0 = ctrl[i];
    float y1 = ctrl[i + 1];

    // Check if segment crosses zero
    if ((y0 <= 0 && y1 >= 0) || (y0 >= 0 && y1 <= 0))
    {
      // Parameter at crossing (in [0,1] of this segment)
      float segT = (std::abs(y1 - y0) > 1e-10f) ? -y0 / (y1 - y0) : 0.5f;

      // Convert to global parameter
      float globalT = (i + segT) / n;
      tMin = std::min(tMin, globalT);
      tMax = std::max(tMax, globalT);
    }
  }

  // Expand slightly for safety
  float eps = 0.01f;
  tMin = std::max(0.0f, tMin - eps);
  tMax = std::min(1.0f, tMax + eps);
}

//! Evaluate a 1D Bézier curve at parameter t using Horner's method
inline float EvalBezier1D(const float* ctrl, int n, float t)
{
  // Use de Casteljau for stability
  float work[BEZIER_MAX_DEGREE + 1];
  for (int i = 0; i <= n; ++i)
    work[i] = ctrl[i];

  for (int j = 1; j <= n; ++j)
  {
    for (int i = 0; i <= n - j; ++i)
    {
      work[i] = (1.0f - t) * work[i] + t * work[i + 1];
    }
  }
  return work[0];
}

//! Ray-Bézier patch intersection using Bézier clipping algorithm
//! Based on Nishita, Sederberg & Kakimoto (1990)
//! @param ctrlPtsX, ctrlPtsY, ctrlPtsZ Bézier control points [(nU)x(nV)] in row-major
//! @param nU, nV Number of control points in U and V directions (= degree + 1)
//! @param rayOrgX/Y/Z Ray origin
//! @param rayDirX/Y/Z Ray direction (normalized)
//! @param uMin, uMax, vMin, vMax Parameter bounds for this patch
//! @param tol Convergence tolerance
//! @return Intersection result
inline BezierClipResult RayBezierClip(
  const float* ctrlPtsX, const float* ctrlPtsY, const float* ctrlPtsZ,
  int nU, int nV,
  float rayOrgX, float rayOrgY, float rayOrgZ,
  float rayDirX, float rayDirY, float rayDirZ,
  float uMin, float uMax, float vMin, float vMax,
  float tol = 1e-6f,
  int maxIter = 20)
{
  BezierClipResult result;
  result.Found = false;

  int degU = nU - 1;  // Degree = nPoints - 1
  int degV = nV - 1;
  int nPts = nU * nV;

  // Step 1: Create two orthogonal planes containing the ray
  // Plane 1: normal = rayDir × someVec
  // Plane 2: normal = rayDir × plane1Normal

  // Find a vector not parallel to ray direction
  float absX = std::abs(rayDirX);
  float absY = std::abs(rayDirY);
  float absZ = std::abs(rayDirZ);

  float perpX, perpY, perpZ;
  if (absX <= absY && absX <= absZ)
  {
    // Use X axis
    perpX = 1; perpY = 0; perpZ = 0;
  }
  else if (absY <= absZ)
  {
    // Use Y axis
    perpX = 0; perpY = 1; perpZ = 0;
  }
  else
  {
    // Use Z axis
    perpX = 0; perpY = 0; perpZ = 1;
  }

  // Plane 1 normal = rayDir × perp
  float n1X = rayDirY * perpZ - rayDirZ * perpY;
  float n1Y = rayDirZ * perpX - rayDirX * perpZ;
  float n1Z = rayDirX * perpY - rayDirY * perpX;

  // Normalize
  float n1Len = std::sqrt(n1X * n1X + n1Y * n1Y + n1Z * n1Z);
  if (n1Len < 1e-10f) return result;
  n1X /= n1Len; n1Y /= n1Len; n1Z /= n1Len;

  // Plane 2 normal = rayDir × n1
  float n2X = rayDirY * n1Z - rayDirZ * n1Y;
  float n2Y = rayDirZ * n1X - rayDirX * n1Z;
  float n2Z = rayDirX * n1Y - rayDirY * n1X;

  // n2 is already normalized (rayDir and n1 are unit and perpendicular)

  // Plane equations: n1·(P - rayOrg) = 0, n2·(P - rayOrg) = 0
  // Rewrite as: n1·P = d1, n2·P = d2
  float d1 = n1X * rayOrgX + n1Y * rayOrgY + n1Z * rayOrgZ;
  float d2 = n2X * rayOrgX + n2Y * rayOrgY + n2Z * rayOrgZ;

  // Step 2: Project control points onto planes (compute signed distances)
  // f(u,v) = n1·S(u,v) - d1 (should be 0 at intersection)
  // g(u,v) = n2·S(u,v) - d2 (should be 0 at intersection)

  float projF[64]; // Max 8x8 control points
  float projG[64];

  if (nPts > 64) return result; // Too many control points

  for (int i = 0; i < nPts; ++i)
  {
    projF[i] = n1X * ctrlPtsX[i] + n1Y * ctrlPtsY[i] + n1Z * ctrlPtsZ[i] - d1;
    projG[i] = n2X * ctrlPtsX[i] + n2Y * ctrlPtsY[i] + n2Z * ctrlPtsZ[i] - d2;
  }

  // Step 3: Iterative Bézier clipping
  float curUMin = 0.0f, curUMax = 1.0f;
  float curVMin = 0.0f, curVMax = 1.0f;

  for (int iter = 0; iter < maxIter; ++iter)
  {
    // Check convergence
    float uSpan = curUMax - curUMin;
    float vSpan = curVMax - curVMin;

    if (uSpan < tol && vSpan < tol)
    {
      // Converged! Compute intersection point
      float u = 0.5f * (curUMin + curUMax);
      float v = 0.5f * (curVMin + curVMax);

      // Evaluate surface at (u, v) using de Casteljau
      // For simplicity, use bilinear for final evaluation
      // (In production, use proper tensor-product de Casteljau)

      // Evaluate in U direction first for each V row
      float rowX[BEZIER_MAX_DEGREE + 1], rowY[BEZIER_MAX_DEGREE + 1], rowZ[BEZIER_MAX_DEGREE + 1];
      for (int j = 0; j <= degV; ++j)
      {
        float workX[BEZIER_MAX_DEGREE + 1], workY[BEZIER_MAX_DEGREE + 1], workZ[BEZIER_MAX_DEGREE + 1];
        for (int i = 0; i <= degU; ++i)
        {
          int idx = j * nU + i;
          workX[i] = ctrlPtsX[idx];
          workY[i] = ctrlPtsY[idx];
          workZ[i] = ctrlPtsZ[idx];
        }
        // de Casteljau in U
        for (int k = 1; k <= degU; ++k)
        {
          for (int i = 0; i <= degU - k; ++i)
          {
            workX[i] = (1 - u) * workX[i] + u * workX[i + 1];
            workY[i] = (1 - u) * workY[i] + u * workY[i + 1];
            workZ[i] = (1 - u) * workZ[i] + u * workZ[i + 1];
          }
        }
        rowX[j] = workX[0];
        rowY[j] = workY[0];
        rowZ[j] = workZ[0];
      }

      // de Casteljau in V
      for (int k = 1; k <= degV; ++k)
      {
        for (int j = 0; j <= degV - k; ++j)
        {
          rowX[j] = (1 - v) * rowX[j] + v * rowX[j + 1];
          rowY[j] = (1 - v) * rowY[j] + v * rowY[j + 1];
          rowZ[j] = (1 - v) * rowZ[j] + v * rowZ[j + 1];
        }
      }

      result.Px = rowX[0];
      result.Py = rowY[0];
      result.Pz = rowZ[0];

      // Compute ray parameter t
      float dx = result.Px - rayOrgX;
      float dy = result.Py - rayOrgY;
      float dz = result.Pz - rayOrgZ;

      // t = (P - O) · D / |D|² (but D is normalized, so |D|² = 1)
      result.T = dx * rayDirX + dy * rayDirY + dz * rayDirZ;

      // Convert local (u,v) to global parameter range
      result.U = uMin + u * (uMax - uMin);
      result.V = vMin + v * (vMax - vMin);
      result.Found = (result.T > 0);

      return result;
    }

    // 2D Bézier clipping: Find parameter bounds where both F and G could be zero
    //
    // For tensor product Bézier surfaces, we need to find where the control net
    // could intersect the f=0 and g=0 planes simultaneously.
    //
    // Simple approach: Find the V range where F has a sign change, and the U range
    // where G has a sign change. Since F varies primarily with V (Y-direction in
    // physical space) and G varies primarily with U (X-direction), we need to:
    // - Clip in V using F (find V range where F crosses zero)
    // - Clip in U using G (find U range where G crosses zero)

    // Clip in V direction using F
    // For each column (fixed U), find where F crosses zero along V
    float globalVClipMinF = 1.0f, globalVClipMaxF = 0.0f;
    bool foundVCrossingF = false;

    for (int col = 0; col <= degU; ++col)
    {
      float fCol[BEZIER_MAX_DEGREE + 1];
      for (int j = 0; j <= degV; ++j)
      {
        fCol[j] = projF[j * nU + col];
      }

      float vClipMin, vClipMax;
      ClipBezier1D(fCol, degV, vClipMin, vClipMax);

      if (vClipMin <= vClipMax)
      {
        globalVClipMinF = std::min(globalVClipMinF, vClipMin);
        globalVClipMaxF = std::max(globalVClipMaxF, vClipMax);
        foundVCrossingF = true;
      }
    }

    if (!foundVCrossingF)
    {
      // F never crosses zero in V direction
      return result;
    }

    // Clip in U direction using G
    // For each row (fixed V), find where G crosses zero along U
    float globalUClipMinG = 1.0f, globalUClipMaxG = 0.0f;
    bool foundUCrossingG = false;

    for (int row = 0; row <= degV; ++row)
    {
      float gRow[BEZIER_MAX_DEGREE + 1];
      for (int i = 0; i <= degU; ++i)
      {
        gRow[i] = projG[row * nU + i];
      }

      float uClipMin, uClipMax;
      ClipBezier1D(gRow, degU, uClipMin, uClipMax);

      if (uClipMin <= uClipMax)
      {
        globalUClipMinG = std::min(globalUClipMinG, uClipMin);
        globalUClipMaxG = std::max(globalUClipMaxG, uClipMax);
        foundUCrossingG = true;
      }
    }

    if (!foundUCrossingG)
    {
      // G never crosses zero in U direction
      return result;
    }

    // Update ranges
    float newUMin = curUMin + globalUClipMinG * (curUMax - curUMin);
    float newUMax = curUMin + globalUClipMaxG * (curUMax - curUMin);
    float newVMin = curVMin + globalVClipMinF * (curVMax - curVMin);
    float newVMax = curVMin + globalVClipMaxF * (curVMax - curVMin);

    // Check if we made progress
    float oldArea = uSpan * vSpan;
    float newArea = (newUMax - newUMin) * (newVMax - newVMin);

    if (newArea > 0.8f * oldArea)
    {
      // Not enough progress - subdivide instead
      // Split at midpoint and recurse on both halves
      // For now, just accept current estimate
      curUMin = newUMin;
      curUMax = newUMax;
      curVMin = newVMin;
      curVMax = newVMax;

      // Force convergence if we're close
      if (uSpan < 0.1f && vSpan < 0.1f)
        break;
    }
    else
    {
      curUMin = newUMin;
      curUMax = newUMax;
      curVMin = newVMin;
      curVMax = newVMax;
    }
  }

  // Max iterations reached - return best estimate if domain is small
  if ((curUMax - curUMin) < 0.1f && (curVMax - curVMin) < 0.1f)
  {
    float u = 0.5f * (curUMin + curUMax);
    float v = 0.5f * (curVMin + curVMax);

    // Quick evaluation (simplified)
    result.U = uMin + u * (uMax - uMin);
    result.V = vMin + v * (vMax - vMin);
    result.Found = false; // Mark as not fully converged
  }

  return result;
}

//=============================================================================
// Custom SIMD Bézier D1 Evaluation for B-spline surfaces
//=============================================================================
// Uses pre-computed Bézier patches to evaluate D1 without OCCT overhead

#ifdef SIMD_SURFACE_NEON

//! Compute cubic Bernstein basis values and derivatives for 4 parameters in parallel
//! @param t     Input: 4 parameter values in [0,1]
//! @param B     Output: B[0..3] = Bernstein basis values B_i(t)
//! @param dB    Output: dB[0..3] = Bernstein basis derivatives B'_i(t)
inline void ComputeBernsteinCubic4_NEON(
  float32x4_t t,
  float32x4_t B[4],
  float32x4_t dB[4])
{
  float32x4_t one = vdupq_n_f32(1.0f);
  float32x4_t two = vdupq_n_f32(2.0f);
  float32x4_t three = vdupq_n_f32(3.0f);
  float32x4_t neg3 = vdupq_n_f32(-3.0f);

  // 1 - t
  float32x4_t omt = vsubq_f32(one, t);
  // (1 - t)^2
  float32x4_t omt2 = vmulq_f32(omt, omt);
  // t^2
  float32x4_t t2 = vmulq_f32(t, t);

  // Bernstein basis values (cubic):
  //   B0(t) = (1-t)^3
  //   B1(t) = 3*t*(1-t)^2
  //   B2(t) = 3*t^2*(1-t)
  //   B3(t) = t^3
  B[0] = vmulq_f32(omt2, omt);                           // (1-t)^3
  B[1] = vmulq_f32(three, vmulq_f32(t, omt2));           // 3*t*(1-t)^2
  B[2] = vmulq_f32(three, vmulq_f32(t2, omt));           // 3*t^2*(1-t)
  B[3] = vmulq_f32(t2, t);                               // t^3

  // Bernstein derivatives:
  //   B'0(t) = -3*(1-t)^2
  //   B'1(t) = 3*(1-t)*(1-3t) = 3*(1-t) - 9*t*(1-t) = 3*(1-t)^2 - 6*t*(1-t)
  //   B'2(t) = 6*t*(1-t) - 3*t^2 = 3*t*(2*(1-t) - t) = 3*t*(2-3t)
  //   B'3(t) = 3*t^2
  dB[0] = vmulq_f32(neg3, omt2);                         // -3*(1-t)^2

  // B'1 = 3*(1-t)^2 - 6*t*(1-t) = 3*(1-t)*((1-t) - 2*t) = 3*(1-t)*(1-3t)
  float32x4_t one_minus_3t = vsubq_f32(one, vmulq_f32(three, t));
  dB[1] = vmulq_f32(three, vmulq_f32(omt, one_minus_3t));

  // B'2 = 3*t*(2-3t)
  float32x4_t two_minus_3t = vsubq_f32(two, vmulq_f32(three, t));
  dB[2] = vmulq_f32(three, vmulq_f32(t, two_minus_3t));

  dB[3] = vmulq_f32(three, t2);                          // 3*t^2
}

//! Evaluate bicubic Bézier patch D1 for 4 (u,v) pairs in parallel
//! Control points are stored in row-major order: P[v_idx * 4 + u_idx]
//! @param ctrlX, ctrlY, ctrlZ  Control points (16 floats each, row-major order)
//! @param u, v                 Parameter values (4 each, in [0,1] local patch coords)
//! @param outPx, outPy, outPz  Output: 4 surface points
//! @param outDuX, outDuY, outDuZ  Output: 4 dP/du derivatives (in local coords)
//! @param outDvX, outDvY, outDvZ  Output: 4 dP/dv derivatives (in local coords)
inline void BezierD1_NEON4(
  const float* ctrlX, const float* ctrlY, const float* ctrlZ,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  // Load 4 u and v values
  float32x4_t vU = vld1q_f32(u);
  float32x4_t vV = vld1q_f32(v);

  // Compute Bernstein basis values and derivatives
  float32x4_t Bu[4], Bv[4];    // Basis values
  float32x4_t dBu[4], dBv[4];  // Derivative values
  ComputeBernsteinCubic4_NEON(vU, Bu, dBu);
  ComputeBernsteinCubic4_NEON(vV, Bv, dBv);

  // Accumulators for P, dP/du, dP/dv
  float32x4_t accPx = vdupq_n_f32(0.0f);
  float32x4_t accPy = vdupq_n_f32(0.0f);
  float32x4_t accPz = vdupq_n_f32(0.0f);
  float32x4_t accDuX = vdupq_n_f32(0.0f);
  float32x4_t accDuY = vdupq_n_f32(0.0f);
  float32x4_t accDuZ = vdupq_n_f32(0.0f);
  float32x4_t accDvX = vdupq_n_f32(0.0f);
  float32x4_t accDvY = vdupq_n_f32(0.0f);
  float32x4_t accDvZ = vdupq_n_f32(0.0f);

  // P(u,v) = ΣᵢΣⱼ Bᵢ(u) * Bⱼ(v) * Pᵢⱼ
  // dP/du  = ΣᵢΣⱼ B'ᵢ(u) * Bⱼ(v) * Pᵢⱼ
  // dP/dv  = ΣᵢΣⱼ Bᵢ(u) * B'ⱼ(v) * Pᵢⱼ
  for (int j = 0; j < 4; ++j) {
    for (int i = 0; i < 4; ++i) {
      int idx = j * 4 + i;  // Row-major: j is V index, i is U index

      // Weights for each component
      float32x4_t w = vmulq_f32(Bu[i], Bv[j]);       // Weight for P
      float32x4_t wDu = vmulq_f32(dBu[i], Bv[j]);    // Weight for dP/du
      float32x4_t wDv = vmulq_f32(Bu[i], dBv[j]);    // Weight for dP/dv

      // Broadcast control point to all 4 lanes
      float32x4_t cx = vdupq_n_f32(ctrlX[idx]);
      float32x4_t cy = vdupq_n_f32(ctrlY[idx]);
      float32x4_t cz = vdupq_n_f32(ctrlZ[idx]);

      // Accumulate: acc += weight * controlPoint
      accPx = vmlaq_f32(accPx, w, cx);
      accPy = vmlaq_f32(accPy, w, cy);
      accPz = vmlaq_f32(accPz, w, cz);

      accDuX = vmlaq_f32(accDuX, wDu, cx);
      accDuY = vmlaq_f32(accDuY, wDu, cy);
      accDuZ = vmlaq_f32(accDuZ, wDu, cz);

      accDvX = vmlaq_f32(accDvX, wDv, cx);
      accDvY = vmlaq_f32(accDvY, wDv, cy);
      accDvZ = vmlaq_f32(accDvZ, wDv, cz);
    }
  }

  // Store results
  vst1q_f32(outPx, accPx);
  vst1q_f32(outPy, accPy);
  vst1q_f32(outPz, accPz);
  vst1q_f32(outDuX, accDuX);
  vst1q_f32(outDuY, accDuY);
  vst1q_f32(outDuZ, accDuZ);
  vst1q_f32(outDvX, accDvX);
  vst1q_f32(outDvY, accDvY);
  vst1q_f32(outDvZ, accDvZ);
}

//! Find which Bézier patch contains the given parameter value
//! @param param  Parameter value to look up
//! @param knots  Knot vector (unique values)
//! @param numKnots  Number of knots
//! @return Index of the patch (0 to numKnots-2)
inline int FindPatchIndex(float param, const float* knots, int numKnots)
{
  // Binary search for the knot span containing param
  // Result: knots[i] <= param < knots[i+1]
  int lo = 0;
  int hi = numKnots - 2;  // Last valid patch index

  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    if (knots[mid] <= param)
      lo = mid;
    else
      hi = mid - 1;
  }

  // Clamp to valid range
  if (lo < 0) lo = 0;
  if (lo > numKnots - 2) lo = numKnots - 2;

  return lo;
}

#endif // SIMD_SURFACE_NEON

//=============================================================================
// SSE implementations for x86/x64
//=============================================================================
#ifdef SIMD_SURFACE_SSE

//! Compute cubic Bernstein basis values and derivatives for 4 parameters (SSE)
inline void ComputeBernsteinCubic4_SSE(
  __m128 t,
  __m128 B[4],
  __m128 dB[4])
{
  __m128 one = _mm_set1_ps(1.0f);
  __m128 two = _mm_set1_ps(2.0f);
  __m128 three = _mm_set1_ps(3.0f);
  __m128 neg3 = _mm_set1_ps(-3.0f);

  // 1 - t
  __m128 omt = _mm_sub_ps(one, t);
  // (1 - t)^2
  __m128 omt2 = _mm_mul_ps(omt, omt);
  // t^2
  __m128 t2 = _mm_mul_ps(t, t);

  // Bernstein basis values
  B[0] = _mm_mul_ps(omt2, omt);                          // (1-t)^3
  B[1] = _mm_mul_ps(three, _mm_mul_ps(t, omt2));         // 3*t*(1-t)^2
  B[2] = _mm_mul_ps(three, _mm_mul_ps(t2, omt));         // 3*t^2*(1-t)
  B[3] = _mm_mul_ps(t2, t);                              // t^3

  // Derivatives
  dB[0] = _mm_mul_ps(neg3, omt2);                        // -3*(1-t)^2

  __m128 one_minus_3t = _mm_sub_ps(one, _mm_mul_ps(three, t));
  dB[1] = _mm_mul_ps(three, _mm_mul_ps(omt, one_minus_3t));

  __m128 two_minus_3t = _mm_sub_ps(two, _mm_mul_ps(three, t));
  dB[2] = _mm_mul_ps(three, _mm_mul_ps(t, two_minus_3t));

  dB[3] = _mm_mul_ps(three, t2);                         // 3*t^2
}

//! Evaluate bicubic Bézier patch D1 for 4 (u,v) pairs (SSE version)
inline void BezierD1_SSE4(
  const float* ctrlX, const float* ctrlY, const float* ctrlZ,
  const float* u, const float* v,
  float* outPx, float* outPy, float* outPz,
  float* outDuX, float* outDuY, float* outDuZ,
  float* outDvX, float* outDvY, float* outDvZ)
{
  __m128 vU = _mm_loadu_ps(u);
  __m128 vV = _mm_loadu_ps(v);

  __m128 Bu[4], Bv[4];
  __m128 dBu[4], dBv[4];
  ComputeBernsteinCubic4_SSE(vU, Bu, dBu);
  ComputeBernsteinCubic4_SSE(vV, Bv, dBv);

  __m128 accPx = _mm_setzero_ps();
  __m128 accPy = _mm_setzero_ps();
  __m128 accPz = _mm_setzero_ps();
  __m128 accDuX = _mm_setzero_ps();
  __m128 accDuY = _mm_setzero_ps();
  __m128 accDuZ = _mm_setzero_ps();
  __m128 accDvX = _mm_setzero_ps();
  __m128 accDvY = _mm_setzero_ps();
  __m128 accDvZ = _mm_setzero_ps();

  for (int j = 0; j < 4; ++j) {
    for (int i = 0; i < 4; ++i) {
      int idx = j * 4 + i;

      __m128 w = _mm_mul_ps(Bu[i], Bv[j]);
      __m128 wDu = _mm_mul_ps(dBu[i], Bv[j]);
      __m128 wDv = _mm_mul_ps(Bu[i], dBv[j]);

      __m128 cx = _mm_set1_ps(ctrlX[idx]);
      __m128 cy = _mm_set1_ps(ctrlY[idx]);
      __m128 cz = _mm_set1_ps(ctrlZ[idx]);

      // SSE doesn't have fused multiply-add before AVX, use mul+add
      accPx = _mm_add_ps(accPx, _mm_mul_ps(w, cx));
      accPy = _mm_add_ps(accPy, _mm_mul_ps(w, cy));
      accPz = _mm_add_ps(accPz, _mm_mul_ps(w, cz));

      accDuX = _mm_add_ps(accDuX, _mm_mul_ps(wDu, cx));
      accDuY = _mm_add_ps(accDuY, _mm_mul_ps(wDu, cy));
      accDuZ = _mm_add_ps(accDuZ, _mm_mul_ps(wDu, cz));

      accDvX = _mm_add_ps(accDvX, _mm_mul_ps(wDv, cx));
      accDvY = _mm_add_ps(accDvY, _mm_mul_ps(wDv, cy));
      accDvZ = _mm_add_ps(accDvZ, _mm_mul_ps(wDv, cz));
    }
  }

  _mm_storeu_ps(outPx, accPx);
  _mm_storeu_ps(outPy, accPy);
  _mm_storeu_ps(outPz, accPz);
  _mm_storeu_ps(outDuX, accDuX);
  _mm_storeu_ps(outDuY, accDuY);
  _mm_storeu_ps(outDuZ, accDuZ);
  _mm_storeu_ps(outDvX, accDvX);
  _mm_storeu_ps(outDvY, accDvY);
  _mm_storeu_ps(outDvZ, accDvZ);
}

#endif // SIMD_SURFACE_SSE

#endif // _BRepIntCurveSurface_SIMDSurface_HeaderFile
