// Created on: 2024-12-09
// Created by: Andrea Pozzetti (with Claude Code assistance)
// Copyright (c) 2024 OPEN CASCADE SAS
//
// Packet ray tracing data structures for coherent ray batching

#ifndef _BRepIntCurveSurface_RayPacket_HeaderFile
#define _BRepIntCurveSurface_RayPacket_HeaderFile

#include <Standard.hxx>
#include <gp_Lin.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <BVH_Types.hxx>
#include <NCollection_Array1.hxx>

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>

// SIMD intrinsics - x86 (SSE/AVX)
#if defined(__SSE__) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 1)
  #define OCCT_RT_HAS_SSE 1
  #include <xmmintrin.h>  // SSE
  #include <emmintrin.h>  // SSE2
#endif

#if defined(__SSE4_1__) || defined(__AVX__)
  #define OCCT_RT_HAS_SSE4 1
  #include <smmintrin.h>  // SSE4.1
#endif

#if defined(__AVX__)
  #define OCCT_RT_HAS_AVX 1
  #include <immintrin.h>  // AVX/AVX2
#endif

// SIMD intrinsics - ARM (NEON)
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
  #define OCCT_RT_HAS_NEON 1
  #include <arm_neon.h>
#endif

//! Ray packet size constants
constexpr int PACKET_SIZE_4  = 4;
constexpr int PACKET_SIZE_8  = 8;
constexpr int PACKET_SIZE_16 = 16;

//! Ray packet with SoA (Structure of Arrays) layout for SIMD-friendly access.
//! Supports packets of 4, 8, or 16 rays for SSE, AVX, and AVX-512 respectively.
template <int N>
struct BRepIntCurveSurface_RayPacket
{
  // Ray origins (SoA layout)
  alignas(64) Standard_Real OrgX[N];
  alignas(64) Standard_Real OrgY[N];
  alignas(64) Standard_Real OrgZ[N];

  // Ray directions (SoA layout)
  alignas(64) Standard_Real DirX[N];
  alignas(64) Standard_Real DirY[N];
  alignas(64) Standard_Real DirZ[N];

  // Precomputed inverse directions for fast ray-box tests
  alignas(64) Standard_Real InvDirX[N];
  alignas(64) Standard_Real InvDirY[N];
  alignas(64) Standard_Real InvDirZ[N];

  // Ray bounds
  alignas(64) Standard_Real TMin[N];
  alignas(64) Standard_Real TMax[N];

  // Original ray indices (for result mapping)
  alignas(64) Standard_Integer RayIndices[N];

  // Active ray mask (bit i = 1 if ray i is active)
  uint32_t ActiveMask;

  // Number of valid rays in this packet (may be < N for last packet)
  Standard_Integer ValidCount;

  // Packet-level bounding frustum (computed from ray corners)
  BVH_Vec3d FrustumMin;
  BVH_Vec3d FrustumMax;

  // Dominant direction sign (for ordered traversal)
  int DirSignX, DirSignY, DirSignZ;

  //! Default constructor
  BRepIntCurveSurface_RayPacket() : ActiveMask(0), ValidCount(0), DirSignX(0), DirSignY(0), DirSignZ(0)
  {
    for (int i = 0; i < N; ++i)
    {
      OrgX[i] = OrgY[i] = OrgZ[i] = 0.0;
      DirX[i] = DirY[i] = 0.0;
      DirZ[i]           = -1.0;
      InvDirX[i] = InvDirY[i] = InvDirZ[i] = 0.0;
      TMin[i]                              = 0.0;
      TMax[i]                              = 1e30;
      RayIndices[i]                        = -1;
    }
    FrustumMin = BVH_Vec3d(1e30, 1e30, 1e30);
    FrustumMax = BVH_Vec3d(-1e30, -1e30, -1e30);
  }

  //! Set ray at index i
  void SetRay(int i, const gp_Lin& ray, Standard_Integer rayIndex)
  {
    if (i < 0 || i >= N)
      return;

    OrgX[i] = ray.Location().X();
    OrgY[i] = ray.Location().Y();
    OrgZ[i] = ray.Location().Z();
    DirX[i] = ray.Direction().X();
    DirY[i] = ray.Direction().Y();
    DirZ[i] = ray.Direction().Z();

    // Precompute inverse direction
    const Standard_Real eps = 1e-12;
    InvDirX[i]              = std::abs(DirX[i]) > eps ? 1.0 / DirX[i] : (DirX[i] >= 0 ? 1.0 / eps : -1.0 / eps);
    InvDirY[i]              = std::abs(DirY[i]) > eps ? 1.0 / DirY[i] : (DirY[i] >= 0 ? 1.0 / eps : -1.0 / eps);
    InvDirZ[i]              = std::abs(DirZ[i]) > eps ? 1.0 / DirZ[i] : (DirZ[i] >= 0 ? 1.0 / eps : -1.0 / eps);

    TMin[i]       = 0.0;
    TMax[i]       = 1e30;
    RayIndices[i] = rayIndex;
    ActiveMask |= (1u << i);
    ValidCount = std::max(ValidCount, i + 1);
  }

  //! Compute the bounding frustum from all active rays
  void ComputeFrustum()
  {
    FrustumMin = BVH_Vec3d(1e30, 1e30, 1e30);
    FrustumMax = BVH_Vec3d(-1e30, -1e30, -1e30);

    // Accumulate direction signs for dominant direction
    int signSumX = 0, signSumY = 0, signSumZ = 0;

    for (int i = 0; i < ValidCount; ++i)
    {
      if (!(ActiveMask & (1u << i)))
        continue;

      // Expand frustum to include ray origin
      FrustumMin[0] = std::min(FrustumMin[0], OrgX[i]);
      FrustumMin[1] = std::min(FrustumMin[1], OrgY[i]);
      FrustumMin[2] = std::min(FrustumMin[2], OrgZ[i]);
      FrustumMax[0] = std::max(FrustumMax[0], OrgX[i]);
      FrustumMax[1] = std::max(FrustumMax[1], OrgY[i]);
      FrustumMax[2] = std::max(FrustumMax[2], OrgZ[i]);

      // Also expand in ray direction (for a reasonable distance)
      Standard_Real extent = 1000.0; // Arbitrary large extent
      FrustumMin[0]        = std::min(FrustumMin[0], OrgX[i] + extent * std::min(0.0, DirX[i]));
      FrustumMin[1]        = std::min(FrustumMin[1], OrgY[i] + extent * std::min(0.0, DirY[i]));
      FrustumMin[2]        = std::min(FrustumMin[2], OrgZ[i] + extent * std::min(0.0, DirZ[i]));
      FrustumMax[0]        = std::max(FrustumMax[0], OrgX[i] + extent * std::max(0.0, DirX[i]));
      FrustumMax[1]        = std::max(FrustumMax[1], OrgY[i] + extent * std::max(0.0, DirY[i]));
      FrustumMax[2]        = std::max(FrustumMax[2], OrgZ[i] + extent * std::max(0.0, DirZ[i]));

      // Accumulate direction signs
      signSumX += (DirX[i] >= 0) ? 1 : -1;
      signSumY += (DirY[i] >= 0) ? 1 : -1;
      signSumZ += (DirZ[i] >= 0) ? 1 : -1;
    }

    // Dominant direction sign (for ordered child traversal)
    DirSignX = (signSumX >= 0) ? 1 : -1;
    DirSignY = (signSumY >= 0) ? 1 : -1;
    DirSignZ = (signSumZ >= 0) ? 1 : -1;
  }

  //! Check if packet is coherent (all rays have similar directions)
  Standard_Boolean IsCoherent(Standard_Real threshold = 0.95) const
  {
    if (ValidCount <= 1)
      return Standard_True;

    // Use first ray as reference
    Standard_Real refX = DirX[0], refY = DirY[0], refZ = DirZ[0];

    for (int i = 1; i < ValidCount; ++i)
    {
      if (!(ActiveMask & (1u << i)))
        continue;

      // Dot product with reference direction
      Standard_Real dot = refX * DirX[i] + refY * DirY[i] + refZ * DirZ[i];
      if (dot < threshold)
        return Standard_False;
    }
    return Standard_True;
  }
};

//! Hit result for a ray packet
template <int N>
struct BRepIntCurveSurface_PacketHitResult
{
  alignas(64) Standard_Real HitT[N];        // Ray parameter at hit
  alignas(64) Standard_Real BaryU[N];       // Barycentric U
  alignas(64) Standard_Real BaryV[N];       // Barycentric V
  alignas(64) Standard_Integer TriIdx[N];   // Triangle index (-1 = no hit)
  alignas(64) Standard_Integer FaceIdx[N];  // Face index (-1 = no hit)

  uint32_t HitMask; // Bit i = 1 if ray i hit something

  BRepIntCurveSurface_PacketHitResult() : HitMask(0)
  {
    for (int i = 0; i < N; ++i)
    {
      HitT[i]    = 1e30;
      BaryU[i]   = 0.0;
      BaryV[i]   = 0.0;
      TriIdx[i]  = -1;
      FaceIdx[i] = -1;
    }
  }
};

//! Ray bin for grouping coherent rays
struct BRepIntCurveSurface_RayBin
{
  Standard_Integer             OctantIndex; // 0-7 based on direction signs
  std::vector<Standard_Integer> RayIndices;  // Indices into original ray array

  BRepIntCurveSurface_RayBin() : OctantIndex(0) {}
};

//! Ray sorter - groups rays by direction for coherent packet formation
class BRepIntCurveSurface_RaySorter
{
public:
  //! Sort rays into bins by direction octant
  void SortByOctant(const NCollection_Array1<gp_Lin>& theRays)
  {
    myBins.clear();
    myBins.resize(8); // 8 octants based on direction sign

    for (int i = 0; i < 8; ++i)
      myBins[i].OctantIndex = i;

    for (Standard_Integer i = theRays.Lower(); i <= theRays.Upper(); ++i)
    {
      const gp_Dir& dir     = theRays(i).Direction();
      int           octant  = ((dir.X() >= 0) ? 0 : 1) | ((dir.Y() >= 0) ? 0 : 2) | ((dir.Z() >= 0) ? 0 : 4);
      myBins[octant].RayIndices.push_back(i);
    }
  }

  //! Sort rays within each bin by Morton code of origin (for spatial coherence)
  void SortBySpatialLocality(const NCollection_Array1<gp_Lin>& theRays, const BVH_Vec3d& sceneMin,
                             const BVH_Vec3d& sceneMax)
  {
    for (auto& bin : myBins)
    {
      if (bin.RayIndices.size() <= 1)
        continue;

      // Sort by Morton code
      std::sort(bin.RayIndices.begin(), bin.RayIndices.end(),
                [&](Standard_Integer a, Standard_Integer b) {
                  return MortonCode(theRays(a).Location(), sceneMin, sceneMax)
                         < MortonCode(theRays(b).Location(), sceneMin, sceneMax);
                });
    }
  }

  //! Create packets from sorted bins
  template <int N>
  void CreatePackets(const NCollection_Array1<gp_Lin>&            theRays,
                     std::vector<BRepIntCurveSurface_RayPacket<N>>& thePackets) const
  {
    thePackets.clear();

    for (const auto& bin : myBins)
    {
      size_t numRays = bin.RayIndices.size();
      if (numRays == 0)
        continue;

      // Create packets from this bin
      for (size_t i = 0; i < numRays; i += N)
      {
        BRepIntCurveSurface_RayPacket<N> packet;

        for (int j = 0; j < N && (i + j) < numRays; ++j)
        {
          Standard_Integer rayIdx = bin.RayIndices[i + j];
          packet.SetRay(j, theRays(rayIdx), rayIdx);
        }

        packet.ComputeFrustum();
        thePackets.push_back(packet);
      }
    }
  }

  //! Get the bins
  const std::vector<BRepIntCurveSurface_RayBin>& GetBins() const { return myBins; }

private:
  //! Compute Morton code for 3D point (for spatial sorting)
  uint64_t MortonCode(const gp_Pnt& pt, const BVH_Vec3d& sceneMin, const BVH_Vec3d& sceneMax) const
  {
    // Normalize to [0, 1] range
    double nx = (pt.X() - sceneMin[0]) / (sceneMax[0] - sceneMin[0] + 1e-10);
    double ny = (pt.Y() - sceneMin[1]) / (sceneMax[1] - sceneMin[1] + 1e-10);
    double nz = (pt.Z() - sceneMin[2]) / (sceneMax[2] - sceneMin[2] + 1e-10);

    // Clamp to [0, 1]
    nx = std::max(0.0, std::min(1.0, nx));
    ny = std::max(0.0, std::min(1.0, ny));
    nz = std::max(0.0, std::min(1.0, nz));

    // Convert to 21-bit integers
    uint64_t ix = static_cast<uint64_t>(nx * 2097151.0);
    uint64_t iy = static_cast<uint64_t>(ny * 2097151.0);
    uint64_t iz = static_cast<uint64_t>(nz * 2097151.0);

    // Interleave bits (Morton code)
    return InterleaveBits(ix) | (InterleaveBits(iy) << 1) | (InterleaveBits(iz) << 2);
  }

  //! Spread bits for Morton code
  uint64_t InterleaveBits(uint64_t x) const
  {
    x = (x | (x << 32)) & 0x1f00000000ffff;
    x = (x | (x << 16)) & 0x1f0000ff0000ff;
    x = (x | (x << 8)) & 0x100f00f00f00f00f;
    x = (x | (x << 4)) & 0x10c30c30c30c30c3;
    x = (x | (x << 2)) & 0x1249249249249249;
    return x;
  }

  std::vector<BRepIntCurveSurface_RayBin> myBins;
};

// Common packet type aliases
using RayPacket4  = BRepIntCurveSurface_RayPacket<PACKET_SIZE_4>;
using RayPacket8  = BRepIntCurveSurface_RayPacket<PACKET_SIZE_8>;
using RayPacket16 = BRepIntCurveSurface_RayPacket<PACKET_SIZE_16>;

using PacketHitResult4  = BRepIntCurveSurface_PacketHitResult<PACKET_SIZE_4>;
using PacketHitResult8  = BRepIntCurveSurface_PacketHitResult<PACKET_SIZE_8>;
using PacketHitResult16 = BRepIntCurveSurface_PacketHitResult<PACKET_SIZE_16>;

//=============================================================================
// SIMD Ray-Box Intersection Functions
//=============================================================================

namespace SIMD
{

#ifdef OCCT_RT_HAS_SSE

//! Test 4 rays against a single box using SSE (float precision)
//! Returns bitmask of rays that hit the box
inline int RayBox4_SSE(const float* orgX, const float* orgY, const float* orgZ,
                       const float* invDirX, const float* invDirY, const float* invDirZ,
                       const float* tMin, const float* tMax,
                       float boxMinX, float boxMinY, float boxMinZ,
                       float boxMaxX, float boxMaxY, float boxMaxZ)
{
  // Load ray data
  __m128 rayOrgX = _mm_loadu_ps(orgX);
  __m128 rayOrgY = _mm_loadu_ps(orgY);
  __m128 rayOrgZ = _mm_loadu_ps(orgZ);
  __m128 rayInvDirX = _mm_loadu_ps(invDirX);
  __m128 rayInvDirY = _mm_loadu_ps(invDirY);
  __m128 rayInvDirZ = _mm_loadu_ps(invDirZ);
  __m128 rayTMin = _mm_loadu_ps(tMin);
  __m128 rayTMax = _mm_loadu_ps(tMax);

  // Broadcast box bounds
  __m128 bMinX = _mm_set1_ps(boxMinX);
  __m128 bMinY = _mm_set1_ps(boxMinY);
  __m128 bMinZ = _mm_set1_ps(boxMinZ);
  __m128 bMaxX = _mm_set1_ps(boxMaxX);
  __m128 bMaxY = _mm_set1_ps(boxMaxY);
  __m128 bMaxZ = _mm_set1_ps(boxMaxZ);

  // Compute t values for X slab
  __m128 t1x = _mm_mul_ps(_mm_sub_ps(bMinX, rayOrgX), rayInvDirX);
  __m128 t2x = _mm_mul_ps(_mm_sub_ps(bMaxX, rayOrgX), rayInvDirX);
  __m128 tNear = _mm_min_ps(t1x, t2x);
  __m128 tFar = _mm_max_ps(t1x, t2x);

  // Compute t values for Y slab
  __m128 t1y = _mm_mul_ps(_mm_sub_ps(bMinY, rayOrgY), rayInvDirY);
  __m128 t2y = _mm_mul_ps(_mm_sub_ps(bMaxY, rayOrgY), rayInvDirY);
  tNear = _mm_max_ps(tNear, _mm_min_ps(t1y, t2y));
  tFar = _mm_min_ps(tFar, _mm_max_ps(t1y, t2y));

  // Compute t values for Z slab
  __m128 t1z = _mm_mul_ps(_mm_sub_ps(bMinZ, rayOrgZ), rayInvDirZ);
  __m128 t2z = _mm_mul_ps(_mm_sub_ps(bMaxZ, rayOrgZ), rayInvDirZ);
  tNear = _mm_max_ps(tNear, _mm_min_ps(t1z, t2z));
  tFar = _mm_min_ps(tFar, _mm_max_ps(t1z, t2z));

  // Check if ray hits box: tNear <= tFar && tFar >= rayTMin && tNear <= rayTMax
  __m128 hitMask = _mm_and_ps(
    _mm_cmple_ps(tNear, tFar),
    _mm_and_ps(_mm_cmpge_ps(tFar, rayTMin), _mm_cmple_ps(tNear, rayTMax))
  );

  return _mm_movemask_ps(hitMask);
}

//! Test 4 rays against a single triangle using SSE (Möller-Trumbore)
//! Returns bitmask of rays that hit, and stores t/u/v in output arrays for hits
inline int RayTriangle4_SSE(const float* orgX, const float* orgY, const float* orgZ,
                            const float* dirX, const float* dirY, const float* dirZ,
                            float v0x, float v0y, float v0z,
                            float v1x, float v1y, float v1z,
                            float v2x, float v2y, float v2z,
                            float* outT, float* outU, float* outV,
                            const float* currentT)
{
  const __m128 eps = _mm_set1_ps(1e-7f);
  const __m128 zero = _mm_setzero_ps();
  const __m128 one = _mm_set1_ps(1.0f);

  // Load ray data
  __m128 rayOrgX = _mm_loadu_ps(orgX);
  __m128 rayOrgY = _mm_loadu_ps(orgY);
  __m128 rayOrgZ = _mm_loadu_ps(orgZ);
  __m128 rayDirX = _mm_loadu_ps(dirX);
  __m128 rayDirY = _mm_loadu_ps(dirY);
  __m128 rayDirZ = _mm_loadu_ps(dirZ);
  __m128 rayT = _mm_loadu_ps(currentT);

  // Edge vectors (broadcast)
  __m128 e1x = _mm_set1_ps(v1x - v0x);
  __m128 e1y = _mm_set1_ps(v1y - v0y);
  __m128 e1z = _mm_set1_ps(v1z - v0z);
  __m128 e2x = _mm_set1_ps(v2x - v0x);
  __m128 e2y = _mm_set1_ps(v2y - v0y);
  __m128 e2z = _mm_set1_ps(v2z - v0z);

  // h = cross(dir, e2)
  __m128 hx = _mm_sub_ps(_mm_mul_ps(rayDirY, e2z), _mm_mul_ps(rayDirZ, e2y));
  __m128 hy = _mm_sub_ps(_mm_mul_ps(rayDirZ, e2x), _mm_mul_ps(rayDirX, e2z));
  __m128 hz = _mm_sub_ps(_mm_mul_ps(rayDirX, e2y), _mm_mul_ps(rayDirY, e2x));

  // a = dot(e1, h)
  __m128 a = _mm_add_ps(_mm_add_ps(_mm_mul_ps(e1x, hx), _mm_mul_ps(e1y, hy)), _mm_mul_ps(e1z, hz));

  // Check for parallel ray (|a| < eps)
  __m128 absA = _mm_andnot_ps(_mm_set1_ps(-0.0f), a);  // abs(a)
  __m128 validMask = _mm_cmpgt_ps(absA, eps);

  // f = 1/a
  __m128 f = _mm_div_ps(one, a);

  // s = org - v0
  __m128 v0X = _mm_set1_ps(v0x);
  __m128 v0Y = _mm_set1_ps(v0y);
  __m128 v0Z = _mm_set1_ps(v0z);
  __m128 sx = _mm_sub_ps(rayOrgX, v0X);
  __m128 sy = _mm_sub_ps(rayOrgY, v0Y);
  __m128 sz = _mm_sub_ps(rayOrgZ, v0Z);

  // u = f * dot(s, h)
  __m128 u = _mm_mul_ps(f, _mm_add_ps(_mm_add_ps(_mm_mul_ps(sx, hx), _mm_mul_ps(sy, hy)), _mm_mul_ps(sz, hz)));

  // Check u bounds [0, 1]
  validMask = _mm_and_ps(validMask, _mm_cmpge_ps(u, zero));
  validMask = _mm_and_ps(validMask, _mm_cmple_ps(u, one));

  // q = cross(s, e1)
  __m128 qx = _mm_sub_ps(_mm_mul_ps(sy, e1z), _mm_mul_ps(sz, e1y));
  __m128 qy = _mm_sub_ps(_mm_mul_ps(sz, e1x), _mm_mul_ps(sx, e1z));
  __m128 qz = _mm_sub_ps(_mm_mul_ps(sx, e1y), _mm_mul_ps(sy, e1x));

  // v = f * dot(dir, q)
  __m128 v = _mm_mul_ps(f, _mm_add_ps(_mm_add_ps(_mm_mul_ps(rayDirX, qx), _mm_mul_ps(rayDirY, qy)), _mm_mul_ps(rayDirZ, qz)));

  // Check v >= 0 and u + v <= 1
  validMask = _mm_and_ps(validMask, _mm_cmpge_ps(v, zero));
  validMask = _mm_and_ps(validMask, _mm_cmple_ps(_mm_add_ps(u, v), one));

  // t = f * dot(e2, q)
  __m128 t = _mm_mul_ps(f, _mm_add_ps(_mm_add_ps(_mm_mul_ps(e2x, qx), _mm_mul_ps(e2y, qy)), _mm_mul_ps(e2z, qz)));

  // Check t > eps && t < currentT
  validMask = _mm_and_ps(validMask, _mm_cmpgt_ps(t, eps));
  validMask = _mm_and_ps(validMask, _mm_cmplt_ps(t, rayT));

  // Store results
  _mm_storeu_ps(outT, t);
  _mm_storeu_ps(outU, u);
  _mm_storeu_ps(outV, v);

  return _mm_movemask_ps(validMask);
}

#endif // OCCT_RT_HAS_SSE

#ifdef OCCT_RT_HAS_AVX

//! Test 8 rays against a single box using AVX (float precision)
//! Returns bitmask of rays that hit the box
inline int RayBox8_AVX(const float* orgX, const float* orgY, const float* orgZ,
                       const float* invDirX, const float* invDirY, const float* invDirZ,
                       const float* tMin, const float* tMax,
                       float boxMinX, float boxMinY, float boxMinZ,
                       float boxMaxX, float boxMaxY, float boxMaxZ)
{
  // Load ray data (8 rays)
  __m256 rayOrgX = _mm256_loadu_ps(orgX);
  __m256 rayOrgY = _mm256_loadu_ps(orgY);
  __m256 rayOrgZ = _mm256_loadu_ps(orgZ);
  __m256 rayInvDirX = _mm256_loadu_ps(invDirX);
  __m256 rayInvDirY = _mm256_loadu_ps(invDirY);
  __m256 rayInvDirZ = _mm256_loadu_ps(invDirZ);
  __m256 rayTMin = _mm256_loadu_ps(tMin);
  __m256 rayTMax = _mm256_loadu_ps(tMax);

  // Broadcast box bounds
  __m256 bMinX = _mm256_set1_ps(boxMinX);
  __m256 bMinY = _mm256_set1_ps(boxMinY);
  __m256 bMinZ = _mm256_set1_ps(boxMinZ);
  __m256 bMaxX = _mm256_set1_ps(boxMaxX);
  __m256 bMaxY = _mm256_set1_ps(boxMaxY);
  __m256 bMaxZ = _mm256_set1_ps(boxMaxZ);

  // Compute t values for X slab
  __m256 t1x = _mm256_mul_ps(_mm256_sub_ps(bMinX, rayOrgX), rayInvDirX);
  __m256 t2x = _mm256_mul_ps(_mm256_sub_ps(bMaxX, rayOrgX), rayInvDirX);
  __m256 tNear = _mm256_min_ps(t1x, t2x);
  __m256 tFar = _mm256_max_ps(t1x, t2x);

  // Compute t values for Y slab
  __m256 t1y = _mm256_mul_ps(_mm256_sub_ps(bMinY, rayOrgY), rayInvDirY);
  __m256 t2y = _mm256_mul_ps(_mm256_sub_ps(bMaxY, rayOrgY), rayInvDirY);
  tNear = _mm256_max_ps(tNear, _mm256_min_ps(t1y, t2y));
  tFar = _mm256_min_ps(tFar, _mm256_max_ps(t1y, t2y));

  // Compute t values for Z slab
  __m256 t1z = _mm256_mul_ps(_mm256_sub_ps(bMinZ, rayOrgZ), rayInvDirZ);
  __m256 t2z = _mm256_mul_ps(_mm256_sub_ps(bMaxZ, rayOrgZ), rayInvDirZ);
  tNear = _mm256_max_ps(tNear, _mm256_min_ps(t1z, t2z));
  tFar = _mm256_min_ps(tFar, _mm256_max_ps(t1z, t2z));

  // Check if ray hits box
  __m256 hitMask = _mm256_and_ps(
    _mm256_cmp_ps(tNear, tFar, _CMP_LE_OQ),
    _mm256_and_ps(_mm256_cmp_ps(tFar, rayTMin, _CMP_GE_OQ), _mm256_cmp_ps(tNear, rayTMax, _CMP_LE_OQ))
  );

  return _mm256_movemask_ps(hitMask);
}

//! Test 8 rays against a single triangle using AVX (Möller-Trumbore)
inline int RayTriangle8_AVX(const float* orgX, const float* orgY, const float* orgZ,
                            const float* dirX, const float* dirY, const float* dirZ,
                            float v0x, float v0y, float v0z,
                            float v1x, float v1y, float v1z,
                            float v2x, float v2y, float v2z,
                            float* outT, float* outU, float* outV,
                            const float* currentT)
{
  const __m256 eps = _mm256_set1_ps(1e-7f);
  const __m256 zero = _mm256_setzero_ps();
  const __m256 one = _mm256_set1_ps(1.0f);

  // Load ray data
  __m256 rayOrgX = _mm256_loadu_ps(orgX);
  __m256 rayOrgY = _mm256_loadu_ps(orgY);
  __m256 rayOrgZ = _mm256_loadu_ps(orgZ);
  __m256 rayDirX = _mm256_loadu_ps(dirX);
  __m256 rayDirY = _mm256_loadu_ps(dirY);
  __m256 rayDirZ = _mm256_loadu_ps(dirZ);
  __m256 rayT = _mm256_loadu_ps(currentT);

  // Edge vectors
  __m256 e1x = _mm256_set1_ps(v1x - v0x);
  __m256 e1y = _mm256_set1_ps(v1y - v0y);
  __m256 e1z = _mm256_set1_ps(v1z - v0z);
  __m256 e2x = _mm256_set1_ps(v2x - v0x);
  __m256 e2y = _mm256_set1_ps(v2y - v0y);
  __m256 e2z = _mm256_set1_ps(v2z - v0z);

  // h = cross(dir, e2)
  __m256 hx = _mm256_sub_ps(_mm256_mul_ps(rayDirY, e2z), _mm256_mul_ps(rayDirZ, e2y));
  __m256 hy = _mm256_sub_ps(_mm256_mul_ps(rayDirZ, e2x), _mm256_mul_ps(rayDirX, e2z));
  __m256 hz = _mm256_sub_ps(_mm256_mul_ps(rayDirX, e2y), _mm256_mul_ps(rayDirY, e2x));

  // a = dot(e1, h)
  __m256 a = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(e1x, hx), _mm256_mul_ps(e1y, hy)), _mm256_mul_ps(e1z, hz));

  // Check for parallel ray
  __m256 absA = _mm256_andnot_ps(_mm256_set1_ps(-0.0f), a);
  __m256 validMask = _mm256_cmp_ps(absA, eps, _CMP_GT_OQ);

  // f = 1/a
  __m256 f = _mm256_div_ps(one, a);

  // s = org - v0
  __m256 v0X = _mm256_set1_ps(v0x);
  __m256 v0Y = _mm256_set1_ps(v0y);
  __m256 v0Z = _mm256_set1_ps(v0z);
  __m256 sx = _mm256_sub_ps(rayOrgX, v0X);
  __m256 sy = _mm256_sub_ps(rayOrgY, v0Y);
  __m256 sz = _mm256_sub_ps(rayOrgZ, v0Z);

  // u = f * dot(s, h)
  __m256 u = _mm256_mul_ps(f, _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(sx, hx), _mm256_mul_ps(sy, hy)), _mm256_mul_ps(sz, hz)));

  // Check u bounds
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(u, zero, _CMP_GE_OQ));
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(u, one, _CMP_LE_OQ));

  // q = cross(s, e1)
  __m256 qx = _mm256_sub_ps(_mm256_mul_ps(sy, e1z), _mm256_mul_ps(sz, e1y));
  __m256 qy = _mm256_sub_ps(_mm256_mul_ps(sz, e1x), _mm256_mul_ps(sx, e1z));
  __m256 qz = _mm256_sub_ps(_mm256_mul_ps(sx, e1y), _mm256_mul_ps(sy, e1x));

  // v = f * dot(dir, q)
  __m256 v = _mm256_mul_ps(f, _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(rayDirX, qx), _mm256_mul_ps(rayDirY, qy)), _mm256_mul_ps(rayDirZ, qz)));

  // Check v bounds
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(v, zero, _CMP_GE_OQ));
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(_mm256_add_ps(u, v), one, _CMP_LE_OQ));

  // t = f * dot(e2, q)
  __m256 t = _mm256_mul_ps(f, _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(e2x, qx), _mm256_mul_ps(e2y, qy)), _mm256_mul_ps(e2z, qz)));

  // Check t bounds
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(t, eps, _CMP_GT_OQ));
  validMask = _mm256_and_ps(validMask, _mm256_cmp_ps(t, rayT, _CMP_LT_OQ));

  // Store results
  _mm256_storeu_ps(outT, t);
  _mm256_storeu_ps(outU, u);
  _mm256_storeu_ps(outV, v);

  return _mm256_movemask_ps(validMask);
}

#endif // OCCT_RT_HAS_AVX

//! Scalar fallback for ray-box test (always available)
inline bool RayBoxScalar(double orgX, double orgY, double orgZ,
                         double invDirX, double invDirY, double invDirZ,
                         double tMin, double tMax,
                         double boxMinX, double boxMinY, double boxMinZ,
                         double boxMaxX, double boxMaxY, double boxMaxZ,
                         double& tNearOut, double& tFarOut)
{
  double t1 = (boxMinX - orgX) * invDirX;
  double t2 = (boxMaxX - orgX) * invDirX;
  double tNear = std::min(t1, t2);
  double tFar = std::max(t1, t2);

  t1 = (boxMinY - orgY) * invDirY;
  t2 = (boxMaxY - orgY) * invDirY;
  tNear = std::max(tNear, std::min(t1, t2));
  tFar = std::min(tFar, std::max(t1, t2));

  t1 = (boxMinZ - orgZ) * invDirZ;
  t2 = (boxMaxZ - orgZ) * invDirZ;
  tNear = std::max(tNear, std::min(t1, t2));
  tFar = std::min(tFar, std::max(t1, t2));

  tNearOut = tNear;
  tFarOut = tFar;

  return tNear <= tFar && tFar >= tMin && tNear <= tMax;
}

} // namespace SIMD

//=============================================================================
// SIMD Newton Refinement Batch Processing
//=============================================================================

//! Structure to hold batch of Newton refinement inputs (4-wide for SSE)
struct alignas(32) NewtonBatch4
{
  // Input UV coordinates (initial guess from triangle intersection)
  float U[4];
  float V[4];

  // Ray origins
  float RayOrgX[4];
  float RayOrgY[4];
  float RayOrgZ[4];

  // Ray directions (normalized)
  float RayDirX[4];
  float RayDirY[4];
  float RayDirZ[4];

  // Surface parameter bounds (per-slot, may differ if different faces)
  float UMin[4], UMax[4];
  float VMin[4], VMax[4];

  // Output: refined UV and T
  float OutU[4];
  float OutV[4];
  float OutT[4];

  // Output: refined point
  float OutPntX[4];
  float OutPntY[4];
  float OutPntZ[4];

  // Output: convergence status (1 = converged, 0 = failed)
  int Converged[4];

  // Output: iteration count
  int IterCount[4];

  // Active mask (bit i = 1 if slot i is active)
  uint32_t ActiveMask;

  // Number of valid slots
  int ValidCount;
};

//! Structure to hold batch of Newton refinement inputs (8-wide for AVX)
struct alignas(64) NewtonBatch8
{
  // Input UV coordinates
  float U[8];
  float V[8];

  // Ray origins
  float RayOrgX[8];
  float RayOrgY[8];
  float RayOrgZ[8];

  // Ray directions
  float RayDirX[8];
  float RayDirY[8];
  float RayDirZ[8];

  // Surface parameter bounds
  float UMin[8], UMax[8];
  float VMin[8], VMax[8];

  // Output
  float OutU[8];
  float OutV[8];
  float OutT[8];
  float OutPntX[8];
  float OutPntY[8];
  float OutPntZ[8];
  int Converged[8];
  int IterCount[8];

  uint32_t ActiveMask;
  int ValidCount;
};

#ifdef OCCT_RT_HAS_SSE

//! SIMD Newton linear algebra for 4 refinements in parallel (SSE)
//! Computes: residual, Jacobian, solves 2x2 system, updates UV
//! Surface evaluations (D1) must be done externally (scalar)
//!
//! @param u Current U values (4 slots)
//! @param v Current V values (4 slots)
//! @param Sx, Sy, Sz Surface point S(u,v) - from D1 call
//! @param dSuX..dSuZ Surface derivative dS/du - from D1 call
//! @param dSvX..dSvZ Surface derivative dS/dv - from D1 call
//! @param rayOrgX..Z Ray origins
//! @param rayDirX..Z Ray directions
//! @param outDu, outDv Output: Newton step (du, dv)
//! @param outT Output: ray parameter t
//! @param outResidual Output: residual magnitude
//! @param activeMask Bitmask of active slots
inline void NewtonStep4_SSE(
  const float* u, const float* v,
  const float* Sx, const float* Sy, const float* Sz,
  const float* dSuX, const float* dSuY, const float* dSuZ,
  const float* dSvX, const float* dSvY, const float* dSvZ,
  const float* rayOrgX, const float* rayOrgY, const float* rayOrgZ,
  const float* rayDirX, const float* rayDirY, const float* rayDirZ,
  float* outDu, float* outDv, float* outT, float* outResidual)
{
  // Load surface points
  __m128 sx = _mm_loadu_ps(Sx);
  __m128 sy = _mm_loadu_ps(Sy);
  __m128 sz = _mm_loadu_ps(Sz);

  // Load surface derivatives
  __m128 dsuX = _mm_loadu_ps(dSuX);
  __m128 dsuY = _mm_loadu_ps(dSuY);
  __m128 dsuZ = _mm_loadu_ps(dSuZ);
  __m128 dsvX = _mm_loadu_ps(dSvX);
  __m128 dsvY = _mm_loadu_ps(dSvY);
  __m128 dsvZ = _mm_loadu_ps(dSvZ);

  // Load ray data
  __m128 orgX = _mm_loadu_ps(rayOrgX);
  __m128 orgY = _mm_loadu_ps(rayOrgY);
  __m128 orgZ = _mm_loadu_ps(rayOrgZ);
  __m128 dirX = _mm_loadu_ps(rayDirX);
  __m128 dirY = _mm_loadu_ps(rayDirY);
  __m128 dirZ = _mm_loadu_ps(rayDirZ);

  // Compute SO = S - O
  __m128 soX = _mm_sub_ps(sx, orgX);
  __m128 soY = _mm_sub_ps(sy, orgY);
  __m128 soZ = _mm_sub_ps(sz, orgZ);

  // Compute t = SO . D (project surface point onto ray)
  __m128 t = _mm_add_ps(_mm_add_ps(_mm_mul_ps(soX, dirX), _mm_mul_ps(soY, dirY)), _mm_mul_ps(soZ, dirZ));
  _mm_storeu_ps(outT, t);

  // Compute residual r = S - (O + t*D)
  __m128 rX = _mm_sub_ps(sx, _mm_add_ps(orgX, _mm_mul_ps(t, dirX)));
  __m128 rY = _mm_sub_ps(sy, _mm_add_ps(orgY, _mm_mul_ps(t, dirY)));
  __m128 rZ = _mm_sub_ps(sz, _mm_add_ps(orgZ, _mm_mul_ps(t, dirZ)));

  // Compute residual magnitude |r|
  __m128 rMagSq = _mm_add_ps(_mm_add_ps(_mm_mul_ps(rX, rX), _mm_mul_ps(rY, rY)), _mm_mul_ps(rZ, rZ));
  __m128 rMag = _mm_sqrt_ps(rMagSq);
  _mm_storeu_ps(outResidual, rMag);

  // Build Jacobian: J = [dSu.dSu  dSu.dSv]
  //                     [dSv.dSu  dSv.dSv]
  __m128 j11 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(dsuX, dsuX), _mm_mul_ps(dsuY, dsuY)), _mm_mul_ps(dsuZ, dsuZ));
  __m128 j12 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(dsuX, dsvX), _mm_mul_ps(dsuY, dsvY)), _mm_mul_ps(dsuZ, dsvZ));
  __m128 j22 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(dsvX, dsvX), _mm_mul_ps(dsvY, dsvY)), _mm_mul_ps(dsvZ, dsvZ));

  // RHS: b = -[r.dSu, r.dSv]
  __m128 b1 = _mm_sub_ps(_mm_setzero_ps(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(rX, dsuX), _mm_mul_ps(rY, dsuY)), _mm_mul_ps(rZ, dsuZ)));
  __m128 b2 = _mm_sub_ps(_mm_setzero_ps(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(rX, dsvX), _mm_mul_ps(rY, dsvY)), _mm_mul_ps(rZ, dsvZ)));

  // Solve 2x2 system via Cramer's rule: det = j11*j22 - j12*j12
  __m128 det = _mm_sub_ps(_mm_mul_ps(j11, j22), _mm_mul_ps(j12, j12));

  // Avoid division by zero
  __m128 eps = _mm_set1_ps(1e-15f);
  __m128 absDet = _mm_andnot_ps(_mm_set1_ps(-0.0f), det);
  __m128 safeDet = _mm_max_ps(absDet, eps);
  __m128 signDet = _mm_and_ps(det, _mm_set1_ps(-0.0f));
  safeDet = _mm_or_ps(safeDet, signDet);  // Preserve sign

  __m128 invDet = _mm_div_ps(_mm_set1_ps(1.0f), safeDet);

  // du = (j22*b1 - j12*b2) / det
  // dv = (j11*b2 - j12*b1) / det
  __m128 du = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(j22, b1), _mm_mul_ps(j12, b2)), invDet);
  __m128 dv = _mm_mul_ps(_mm_sub_ps(_mm_mul_ps(j11, b2), _mm_mul_ps(j12, b1)), invDet);

  // Damping: if |step| > 0.5, scale to 0.5
  __m128 stepSq = _mm_add_ps(_mm_mul_ps(du, du), _mm_mul_ps(dv, dv));
  __m128 stepMag = _mm_sqrt_ps(stepSq);
  __m128 maxStep = _mm_set1_ps(0.5f);
  __m128 needDamp = _mm_cmpgt_ps(stepMag, maxStep);
  __m128 scale = _mm_div_ps(maxStep, _mm_max_ps(stepMag, eps));
  du = _mm_blendv_ps(du, _mm_mul_ps(du, scale), needDamp);
  dv = _mm_blendv_ps(dv, _mm_mul_ps(dv, scale), needDamp);

  _mm_storeu_ps(outDu, du);
  _mm_storeu_ps(outDv, dv);
}

#endif // OCCT_RT_HAS_SSE

#ifdef OCCT_RT_HAS_NEON

//! SIMD Newton linear algebra for 4 refinements in parallel (ARM NEON)
//! Equivalent to NewtonStep4_SSE but using ARM NEON intrinsics
inline void NewtonStep4_NEON(
  const float* u, const float* v,
  const float* Sx, const float* Sy, const float* Sz,
  const float* dSuX, const float* dSuY, const float* dSuZ,
  const float* dSvX, const float* dSvY, const float* dSvZ,
  const float* rayOrgX, const float* rayOrgY, const float* rayOrgZ,
  const float* rayDirX, const float* rayDirY, const float* rayDirZ,
  float* outDu, float* outDv, float* outT, float* outResidual)
{
  // Load surface points
  float32x4_t sx = vld1q_f32(Sx);
  float32x4_t sy = vld1q_f32(Sy);
  float32x4_t sz = vld1q_f32(Sz);

  // Load surface derivatives
  float32x4_t dsuX = vld1q_f32(dSuX);
  float32x4_t dsuY = vld1q_f32(dSuY);
  float32x4_t dsuZ = vld1q_f32(dSuZ);
  float32x4_t dsvX = vld1q_f32(dSvX);
  float32x4_t dsvY = vld1q_f32(dSvY);
  float32x4_t dsvZ = vld1q_f32(dSvZ);

  // Load ray data
  float32x4_t orgX = vld1q_f32(rayOrgX);
  float32x4_t orgY = vld1q_f32(rayOrgY);
  float32x4_t orgZ = vld1q_f32(rayOrgZ);
  float32x4_t dirX = vld1q_f32(rayDirX);
  float32x4_t dirY = vld1q_f32(rayDirY);
  float32x4_t dirZ = vld1q_f32(rayDirZ);

  // Compute SO = S - O
  float32x4_t soX = vsubq_f32(sx, orgX);
  float32x4_t soY = vsubq_f32(sy, orgY);
  float32x4_t soZ = vsubq_f32(sz, orgZ);

  // Compute t = SO . D (project surface point onto ray)
  float32x4_t t = vmlaq_f32(vmlaq_f32(vmulq_f32(soX, dirX), soY, dirY), soZ, dirZ);
  vst1q_f32(outT, t);

  // Compute residual r = S - (O + t*D)
  float32x4_t rX = vsubq_f32(sx, vmlaq_f32(orgX, t, dirX));
  float32x4_t rY = vsubq_f32(sy, vmlaq_f32(orgY, t, dirY));
  float32x4_t rZ = vsubq_f32(sz, vmlaq_f32(orgZ, t, dirZ));

  // Compute residual magnitude |r|
  float32x4_t rMagSq = vmlaq_f32(vmlaq_f32(vmulq_f32(rX, rX), rY, rY), rZ, rZ);
  float32x4_t rMag = vsqrtq_f32(rMagSq);
  vst1q_f32(outResidual, rMag);

  // Build Jacobian: J = [dSu.dSu  dSu.dSv]
  //                     [dSv.dSu  dSv.dSv]
  float32x4_t j11 = vmlaq_f32(vmlaq_f32(vmulq_f32(dsuX, dsuX), dsuY, dsuY), dsuZ, dsuZ);
  float32x4_t j12 = vmlaq_f32(vmlaq_f32(vmulq_f32(dsuX, dsvX), dsuY, dsvY), dsuZ, dsvZ);
  float32x4_t j22 = vmlaq_f32(vmlaq_f32(vmulq_f32(dsvX, dsvX), dsvY, dsvY), dsvZ, dsvZ);

  // RHS: b = -[r.dSu, r.dSv]
  float32x4_t b1 = vnegq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(rX, dsuX), rY, dsuY), rZ, dsuZ));
  float32x4_t b2 = vnegq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(rX, dsvX), rY, dsvY), rZ, dsvZ));

  // Solve 2x2 system via Cramer's rule: det = j11*j22 - j12*j12
  float32x4_t det = vmlsq_f32(vmulq_f32(j11, j22), j12, j12);

  // Avoid division by zero
  float32x4_t eps = vdupq_n_f32(1e-15f);
  float32x4_t absDet = vabsq_f32(det);
  float32x4_t safeDet = vmaxq_f32(absDet, eps);
  // Preserve sign: safeDet = copysign(safeDet, det)
  uint32x4_t signMask = vdupq_n_u32(0x80000000);
  uint32x4_t detSign = vandq_u32(vreinterpretq_u32_f32(det), signMask);
  safeDet = vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(safeDet), detSign));

  float32x4_t invDet = vdivq_f32(vdupq_n_f32(1.0f), safeDet);

  // du = (j22*b1 - j12*b2) / det
  // dv = (j11*b2 - j12*b1) / det
  float32x4_t du = vmulq_f32(vmlsq_f32(vmulq_f32(j22, b1), j12, b2), invDet);
  float32x4_t dv = vmulq_f32(vmlsq_f32(vmulq_f32(j11, b2), j12, b1), invDet);

  // Damping: if |step| > 0.5, scale to 0.5
  float32x4_t stepSq = vmlaq_f32(vmulq_f32(du, du), dv, dv);
  float32x4_t stepMag = vsqrtq_f32(stepSq);
  float32x4_t maxStep = vdupq_n_f32(0.5f);
  uint32x4_t needDamp = vcgtq_f32(stepMag, maxStep);
  float32x4_t scale = vdivq_f32(maxStep, vmaxq_f32(stepMag, eps));
  // Blend: if needDamp, use scaled value, else use original
  du = vbslq_f32(needDamp, vmulq_f32(du, scale), du);
  dv = vbslq_f32(needDamp, vmulq_f32(dv, scale), dv);

  vst1q_f32(outDu, du);
  vst1q_f32(outDv, dv);
}

#endif // OCCT_RT_HAS_NEON

#ifdef OCCT_RT_HAS_AVX

//! SIMD Newton linear algebra for 8 refinements in parallel (AVX)
inline void NewtonStep8_AVX(
  const float* u, const float* v,
  const float* Sx, const float* Sy, const float* Sz,
  const float* dSuX, const float* dSuY, const float* dSuZ,
  const float* dSvX, const float* dSvY, const float* dSvZ,
  const float* rayOrgX, const float* rayOrgY, const float* rayOrgZ,
  const float* rayDirX, const float* rayDirY, const float* rayDirZ,
  float* outDu, float* outDv, float* outT, float* outResidual)
{
  // Load surface points
  __m256 sx = _mm256_loadu_ps(Sx);
  __m256 sy = _mm256_loadu_ps(Sy);
  __m256 sz = _mm256_loadu_ps(Sz);

  // Load surface derivatives
  __m256 dsuX = _mm256_loadu_ps(dSuX);
  __m256 dsuY = _mm256_loadu_ps(dSuY);
  __m256 dsuZ = _mm256_loadu_ps(dSuZ);
  __m256 dsvX = _mm256_loadu_ps(dSvX);
  __m256 dsvY = _mm256_loadu_ps(dSvY);
  __m256 dsvZ = _mm256_loadu_ps(dSvZ);

  // Load ray data
  __m256 orgX = _mm256_loadu_ps(rayOrgX);
  __m256 orgY = _mm256_loadu_ps(rayOrgY);
  __m256 orgZ = _mm256_loadu_ps(rayOrgZ);
  __m256 dirX = _mm256_loadu_ps(rayDirX);
  __m256 dirY = _mm256_loadu_ps(rayDirY);
  __m256 dirZ = _mm256_loadu_ps(rayDirZ);

  // Compute SO = S - O
  __m256 soX = _mm256_sub_ps(sx, orgX);
  __m256 soY = _mm256_sub_ps(sy, orgY);
  __m256 soZ = _mm256_sub_ps(sz, orgZ);

  // Compute t = SO . D
  __m256 t = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(soX, dirX), _mm256_mul_ps(soY, dirY)), _mm256_mul_ps(soZ, dirZ));
  _mm256_storeu_ps(outT, t);

  // Compute residual r = S - (O + t*D)
  __m256 rX = _mm256_sub_ps(sx, _mm256_add_ps(orgX, _mm256_mul_ps(t, dirX)));
  __m256 rY = _mm256_sub_ps(sy, _mm256_add_ps(orgY, _mm256_mul_ps(t, dirY)));
  __m256 rZ = _mm256_sub_ps(sz, _mm256_add_ps(orgZ, _mm256_mul_ps(t, dirZ)));

  // Compute residual magnitude
  __m256 rMagSq = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(rX, rX), _mm256_mul_ps(rY, rY)), _mm256_mul_ps(rZ, rZ));
  __m256 rMag = _mm256_sqrt_ps(rMagSq);
  _mm256_storeu_ps(outResidual, rMag);

  // Build Jacobian
  __m256 j11 = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(dsuX, dsuX), _mm256_mul_ps(dsuY, dsuY)), _mm256_mul_ps(dsuZ, dsuZ));
  __m256 j12 = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(dsuX, dsvX), _mm256_mul_ps(dsuY, dsvY)), _mm256_mul_ps(dsuZ, dsvZ));
  __m256 j22 = _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(dsvX, dsvX), _mm256_mul_ps(dsvY, dsvY)), _mm256_mul_ps(dsvZ, dsvZ));

  // RHS
  __m256 b1 = _mm256_sub_ps(_mm256_setzero_ps(), _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(rX, dsuX), _mm256_mul_ps(rY, dsuY)), _mm256_mul_ps(rZ, dsuZ)));
  __m256 b2 = _mm256_sub_ps(_mm256_setzero_ps(), _mm256_add_ps(_mm256_add_ps(_mm256_mul_ps(rX, dsvX), _mm256_mul_ps(rY, dsvY)), _mm256_mul_ps(rZ, dsvZ)));

  // Solve 2x2 system
  __m256 det = _mm256_sub_ps(_mm256_mul_ps(j11, j22), _mm256_mul_ps(j12, j12));
  __m256 eps = _mm256_set1_ps(1e-15f);
  __m256 absDet = _mm256_andnot_ps(_mm256_set1_ps(-0.0f), det);
  __m256 safeDet = _mm256_max_ps(absDet, eps);
  __m256 signDet = _mm256_and_ps(det, _mm256_set1_ps(-0.0f));
  safeDet = _mm256_or_ps(safeDet, signDet);

  __m256 invDet = _mm256_div_ps(_mm256_set1_ps(1.0f), safeDet);

  __m256 du = _mm256_mul_ps(_mm256_sub_ps(_mm256_mul_ps(j22, b1), _mm256_mul_ps(j12, b2)), invDet);
  __m256 dv = _mm256_mul_ps(_mm256_sub_ps(_mm256_mul_ps(j11, b2), _mm256_mul_ps(j12, b1)), invDet);

  // Damping
  __m256 stepSq = _mm256_add_ps(_mm256_mul_ps(du, du), _mm256_mul_ps(dv, dv));
  __m256 stepMag = _mm256_sqrt_ps(stepSq);
  __m256 maxStep = _mm256_set1_ps(0.5f);
  __m256 needDamp = _mm256_cmp_ps(stepMag, maxStep, _CMP_GT_OQ);
  __m256 scale = _mm256_div_ps(maxStep, _mm256_max_ps(stepMag, eps));
  du = _mm256_blendv_ps(du, _mm256_mul_ps(du, scale), needDamp);
  dv = _mm256_blendv_ps(dv, _mm256_mul_ps(dv, scale), needDamp);

  _mm256_storeu_ps(outDu, du);
  _mm256_storeu_ps(outDv, dv);
}

#endif // OCCT_RT_HAS_AVX

//=============================================================================
// Batch Newton Iteration Loop with SIMD Acceleration
//=============================================================================

//! Result codes for batch Newton refinement
enum class NewtonBatchStatus
{
  AllConverged,    //!< All slots converged
  PartialConverge, //!< Some slots converged
  NoneConverged    //!< No slots converged
};

//! Configuration for batch Newton refinement
struct NewtonBatchConfig
{
  float  Tolerance   = 1e-5f;  //!< Convergence tolerance
  int    MaxIter     = 5;      //!< Maximum iterations
  float  RelaxedTol  = 1e-3f;  //!< Relaxed tolerance for early exit (after 2 iters)
  float  StallFactor = 0.8f;   //!< Stall detection factor (0.8 = 20% improvement required)
};

#ifdef OCCT_RT_HAS_SSE

//! Run Newton iteration loop for 4 refinements using SSE
//! Surface D1 evaluations are done via callback (not vectorizable)
//!
//! @param batch Input/output batch structure
//! @param surfaceD1 Callback to evaluate D1(u,v) -> (Pnt, dSu, dSv)
//!                  Signature: void(int slot, float u, float v,
//!                             float& Sx, float& Sy, float& Sz,
//!                             float& dSuX, float& dSuY, float& dSuZ,
//!                             float& dSvX, float& dSvY, float& dSvZ)
//! @param config Newton configuration
//! @return Status indicating convergence
template <typename SurfaceD1Func>
NewtonBatchStatus NewtonIterateBatch4_SSE(
  NewtonBatch4& batch,
  SurfaceD1Func surfaceD1,
  const NewtonBatchConfig& config = NewtonBatchConfig())
{
  // Working arrays for surface evaluation (SoA layout)
  alignas(16) float Sx[4], Sy[4], Sz[4];
  alignas(16) float dSuX[4], dSuY[4], dSuZ[4];
  alignas(16) float dSvX[4], dSvY[4], dSvZ[4];
  alignas(16) float du[4], dv[4], t[4], residual[4];
  alignas(16) float prevResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestU[4], bestV[4], bestT[4];
  alignas(16) float bestPntX[4], bestPntY[4], bestPntZ[4];

  // Initialize from input
  for (int i = 0; i < 4; ++i)
  {
    bestU[i] = batch.U[i];
    bestV[i] = batch.V[i];
    bestT[i] = 0.0f;
    batch.Converged[i] = 0;
    batch.IterCount[i] = 0;
  }

  uint32_t activeMask = batch.ActiveMask & 0xF;
  int convergedCount = 0;

  for (int iter = 0; iter < config.MaxIter && activeMask != 0; ++iter)
  {
    // Serial D1 evaluation for each active slot
    for (int i = 0; i < 4; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      surfaceD1(i, batch.U[i], batch.V[i],
                Sx[i], Sy[i], Sz[i],
                dSuX[i], dSuY[i], dSuZ[i],
                dSvX[i], dSvY[i], dSvZ[i]);
      batch.IterCount[i]++;
    }

    // SIMD Newton step
    NewtonStep4_SSE(
      batch.U, batch.V,
      Sx, Sy, Sz,
      dSuX, dSuY, dSuZ,
      dSvX, dSvY, dSvZ,
      batch.RayOrgX, batch.RayOrgY, batch.RayOrgZ,
      batch.RayDirX, batch.RayDirY, batch.RayDirZ,
      du, dv, t, residual);

    // Check convergence and update (scalar - convergence logic is complex)
    for (int i = 0; i < 4; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      // Track best result
      if (residual[i] < bestResidual[i])
      {
        bestResidual[i] = residual[i];
        bestU[i] = batch.U[i];
        bestV[i] = batch.V[i];
        bestT[i] = t[i];
        bestPntX[i] = Sx[i];
        bestPntY[i] = Sy[i];
        bestPntZ[i] = Sz[i];
      }

      // Check full convergence
      if (residual[i] < config.Tolerance)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Relaxed early exit after 2 iterations
      if (iter >= 2 && residual[i] < config.RelaxedTol)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Stall detection
      if (iter >= 1 && residual[i] > prevResidual[i] * config.StallFactor)
      {
        // Use best result
        if (bestResidual[i] < config.RelaxedTol)
        {
          batch.Converged[i] = 1;
          batch.OutU[i] = bestU[i];
          batch.OutV[i] = bestV[i];
          batch.OutT[i] = bestT[i];
          batch.OutPntX[i] = bestPntX[i];
          batch.OutPntY[i] = bestPntY[i];
          batch.OutPntZ[i] = bestPntZ[i];
          convergedCount++;
        }
        activeMask &= ~(1u << i);
        continue;
      }

      prevResidual[i] = residual[i];

      // Update UV with clamping
      batch.U[i] += du[i];
      batch.V[i] += dv[i];
      batch.U[i] = std::max(batch.UMin[i], std::min(batch.UMax[i], batch.U[i]));
      batch.V[i] = std::max(batch.VMin[i], std::min(batch.VMax[i], batch.V[i]));
    }
  }

  // Handle remaining active slots (didn't converge)
  for (int i = 0; i < 4; ++i)
  {
    if (activeMask & (1u << i))
    {
      // Use best result if acceptable
      if (bestResidual[i] < config.RelaxedTol * 10.0f)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = bestU[i];
        batch.OutV[i] = bestV[i];
        batch.OutT[i] = bestT[i];
        batch.OutPntX[i] = bestPntX[i];
        batch.OutPntY[i] = bestPntY[i];
        batch.OutPntZ[i] = bestPntZ[i];
        convergedCount++;
      }
    }
  }

  if (convergedCount == batch.ValidCount)
    return NewtonBatchStatus::AllConverged;
  else if (convergedCount > 0)
    return NewtonBatchStatus::PartialConverge;
  else
    return NewtonBatchStatus::NoneConverged;
}

#endif // OCCT_RT_HAS_SSE

#ifdef OCCT_RT_HAS_NEON

//! Run Newton iteration loop for 4 refinements using ARM NEON
template <typename SurfaceD1Func>
NewtonBatchStatus NewtonIterateBatch4_NEON(
  NewtonBatch4& batch,
  SurfaceD1Func surfaceD1,
  const NewtonBatchConfig& config = NewtonBatchConfig())
{
  // Working arrays for surface evaluation (SoA layout)
  alignas(16) float Sx[4], Sy[4], Sz[4];
  alignas(16) float dSuX[4], dSuY[4], dSuZ[4];
  alignas(16) float dSvX[4], dSvY[4], dSvZ[4];
  alignas(16) float du[4], dv[4], t[4], residual[4];
  alignas(16) float prevResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestU[4], bestV[4], bestT[4];
  alignas(16) float bestPntX[4], bestPntY[4], bestPntZ[4];

  // Initialize from input
  for (int i = 0; i < 4; ++i)
  {
    bestU[i] = batch.U[i];
    bestV[i] = batch.V[i];
    bestT[i] = 0.0f;
    batch.Converged[i] = 0;
    batch.IterCount[i] = 0;
  }

  uint32_t activeMask = batch.ActiveMask & 0xF;
  int convergedCount = 0;

  for (int iter = 0; iter < config.MaxIter && activeMask != 0; ++iter)
  {
    // Serial D1 evaluation for each active slot
    for (int i = 0; i < 4; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      surfaceD1(i, batch.U[i], batch.V[i],
                Sx[i], Sy[i], Sz[i],
                dSuX[i], dSuY[i], dSuZ[i],
                dSvX[i], dSvY[i], dSvZ[i]);
      batch.IterCount[i]++;
    }

    // SIMD Newton step using NEON
    NewtonStep4_NEON(
      batch.U, batch.V,
      Sx, Sy, Sz,
      dSuX, dSuY, dSuZ,
      dSvX, dSvY, dSvZ,
      batch.RayOrgX, batch.RayOrgY, batch.RayOrgZ,
      batch.RayDirX, batch.RayDirY, batch.RayDirZ,
      du, dv, t, residual);

    // Check convergence and update (scalar - convergence logic is complex)
    for (int i = 0; i < 4; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      // Track best result
      if (residual[i] < bestResidual[i])
      {
        bestResidual[i] = residual[i];
        bestU[i] = batch.U[i];
        bestV[i] = batch.V[i];
        bestT[i] = t[i];
        bestPntX[i] = Sx[i];
        bestPntY[i] = Sy[i];
        bestPntZ[i] = Sz[i];
      }

      // Check full convergence
      if (residual[i] < config.Tolerance)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Relaxed early exit after 2 iterations
      if (iter >= 2 && residual[i] < config.RelaxedTol)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Stall detection
      if (iter >= 1 && residual[i] > prevResidual[i] * config.StallFactor)
      {
        // Use best result
        if (bestResidual[i] < config.RelaxedTol)
        {
          batch.Converged[i] = 1;
          batch.OutU[i] = bestU[i];
          batch.OutV[i] = bestV[i];
          batch.OutT[i] = bestT[i];
          batch.OutPntX[i] = bestPntX[i];
          batch.OutPntY[i] = bestPntY[i];
          batch.OutPntZ[i] = bestPntZ[i];
          convergedCount++;
        }
        activeMask &= ~(1u << i);
        continue;
      }

      prevResidual[i] = residual[i];

      // Update UV with clamping
      batch.U[i] += du[i];
      batch.V[i] += dv[i];
      batch.U[i] = std::max(batch.UMin[i], std::min(batch.UMax[i], batch.U[i]));
      batch.V[i] = std::max(batch.VMin[i], std::min(batch.VMax[i], batch.V[i]));
    }
  }

  // Handle remaining active slots (didn't converge)
  for (int i = 0; i < 4; ++i)
  {
    if (activeMask & (1u << i))
    {
      // Use best result if acceptable
      if (bestResidual[i] < config.RelaxedTol * 10.0f)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = bestU[i];
        batch.OutV[i] = bestV[i];
        batch.OutT[i] = bestT[i];
        batch.OutPntX[i] = bestPntX[i];
        batch.OutPntY[i] = bestPntY[i];
        batch.OutPntZ[i] = bestPntZ[i];
        convergedCount++;
      }
    }
  }

  if (convergedCount == batch.ValidCount)
    return NewtonBatchStatus::AllConverged;
  else if (convergedCount > 0)
    return NewtonBatchStatus::PartialConverge;
  else
    return NewtonBatchStatus::NoneConverged;
}

//! Run Newton iteration loop using fully vectorized D1 evaluation (NEON)
//! @param simdSurfD1 - A callable that takes (u[4], v[4]) and fills (Px[4], Py[4], ... dSvZ[4])
//!                     using SIMD surface evaluation (e.g., TorusD1_NEON4, SphereD1_NEON4)
template <typename SIMDSurfaceD1Func, typename ScalarD1Func>
NewtonBatchStatus NewtonIterateBatch4_NEON_SIMDSurf(
  NewtonBatch4& batch,
  SIMDSurfaceD1Func simdSurfD1,  // Processes all 4 at once
  ScalarD1Func scalarD1,          // Fallback for inactive slots
  const NewtonBatchConfig& config = NewtonBatchConfig())
{
  // Working arrays for surface evaluation (SoA layout)
  alignas(16) float Sx[4], Sy[4], Sz[4];
  alignas(16) float dSuX[4], dSuY[4], dSuZ[4];
  alignas(16) float dSvX[4], dSvY[4], dSvZ[4];
  alignas(16) float du[4], dv[4], t[4], residual[4];
  alignas(16) float prevResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestResidual[4] = {1e30f, 1e30f, 1e30f, 1e30f};
  alignas(16) float bestU[4], bestV[4], bestT[4];
  alignas(16) float bestPntX[4], bestPntY[4], bestPntZ[4];

  // Initialize from input
  for (int i = 0; i < 4; ++i)
  {
    bestU[i] = batch.U[i];
    bestV[i] = batch.V[i];
    bestT[i] = 0.0f;
    batch.Converged[i] = 0;
    batch.IterCount[i] = 0;
  }

  uint32_t activeMask = batch.ActiveMask & 0xF;
  int convergedCount = 0;

  for (int iter = 0; iter < config.MaxIter && activeMask != 0; ++iter)
  {
    // SIMD D1 evaluation - process all 4 at once!
    simdSurfD1(batch.U, batch.V,
               Sx, Sy, Sz,
               dSuX, dSuY, dSuZ,
               dSvX, dSvY, dSvZ);

    // Update iteration count for active slots
    for (int i = 0; i < 4; ++i)
    {
      if (activeMask & (1u << i))
        batch.IterCount[i]++;
    }

    // SIMD Newton step using NEON
    NewtonStep4_NEON(
      batch.U, batch.V,
      Sx, Sy, Sz,
      dSuX, dSuY, dSuZ,
      dSvX, dSvY, dSvZ,
      batch.RayOrgX, batch.RayOrgY, batch.RayOrgZ,
      batch.RayDirX, batch.RayDirY, batch.RayDirZ,
      du, dv, t, residual);

    // Check convergence and update (scalar - convergence logic is complex)
    for (int i = 0; i < 4; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      // Track best result
      if (residual[i] < bestResidual[i])
      {
        bestResidual[i] = residual[i];
        bestU[i] = batch.U[i];
        bestV[i] = batch.V[i];
        bestT[i] = t[i];
        bestPntX[i] = Sx[i];
        bestPntY[i] = Sy[i];
        bestPntZ[i] = Sz[i];
      }

      // Check full convergence
      if (residual[i] < config.Tolerance)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Relaxed early exit after 2 iterations
      if (iter >= 2 && residual[i] < config.RelaxedTol)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Stall detection
      if (iter >= 1 && residual[i] > prevResidual[i] * config.StallFactor)
      {
        // Use best result
        if (bestResidual[i] < config.RelaxedTol)
        {
          batch.Converged[i] = 1;
          batch.OutU[i] = bestU[i];
          batch.OutV[i] = bestV[i];
          batch.OutT[i] = bestT[i];
          batch.OutPntX[i] = bestPntX[i];
          batch.OutPntY[i] = bestPntY[i];
          batch.OutPntZ[i] = bestPntZ[i];
          convergedCount++;
        }
        activeMask &= ~(1u << i);
        continue;
      }

      prevResidual[i] = residual[i];

      // Update UV with clamping
      batch.U[i] += du[i];
      batch.V[i] += dv[i];
      batch.U[i] = std::max(batch.UMin[i], std::min(batch.UMax[i], batch.U[i]));
      batch.V[i] = std::max(batch.VMin[i], std::min(batch.VMax[i], batch.V[i]));
    }
  }

  // Handle remaining active slots (didn't converge)
  for (int i = 0; i < 4; ++i)
  {
    if (activeMask & (1u << i))
    {
      // Use best result if acceptable
      if (bestResidual[i] < config.RelaxedTol * 10.0f)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = bestU[i];
        batch.OutV[i] = bestV[i];
        batch.OutT[i] = bestT[i];
        batch.OutPntX[i] = bestPntX[i];
        batch.OutPntY[i] = bestPntY[i];
        batch.OutPntZ[i] = bestPntZ[i];
        convergedCount++;
      }
    }
  }

  if (convergedCount == batch.ValidCount)
    return NewtonBatchStatus::AllConverged;
  else if (convergedCount > 0)
    return NewtonBatchStatus::PartialConverge;
  else
    return NewtonBatchStatus::NoneConverged;
}

#endif // OCCT_RT_HAS_NEON

#ifdef OCCT_RT_HAS_AVX

//! Run Newton iteration loop for 8 refinements using AVX
template <typename SurfaceD1Func>
NewtonBatchStatus NewtonIterateBatch8_AVX(
  NewtonBatch8& batch,
  SurfaceD1Func surfaceD1,
  const NewtonBatchConfig& config = NewtonBatchConfig())
{
  // Working arrays for surface evaluation (SoA layout)
  alignas(32) float Sx[8], Sy[8], Sz[8];
  alignas(32) float dSuX[8], dSuY[8], dSuZ[8];
  alignas(32) float dSvX[8], dSvY[8], dSvZ[8];
  alignas(32) float du[8], dv[8], t[8], residual[8];
  alignas(32) float prevResidual[8] = {1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f};
  alignas(32) float bestResidual[8] = {1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f, 1e30f};
  alignas(32) float bestU[8], bestV[8], bestT[8];
  alignas(32) float bestPntX[8], bestPntY[8], bestPntZ[8];

  // Initialize from input
  for (int i = 0; i < 8; ++i)
  {
    bestU[i] = batch.U[i];
    bestV[i] = batch.V[i];
    bestT[i] = 0.0f;
    batch.Converged[i] = 0;
    batch.IterCount[i] = 0;
  }

  uint32_t activeMask = batch.ActiveMask & 0xFF;
  int convergedCount = 0;

  for (int iter = 0; iter < config.MaxIter && activeMask != 0; ++iter)
  {
    // Serial D1 evaluation for each active slot
    for (int i = 0; i < 8; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      surfaceD1(i, batch.U[i], batch.V[i],
                Sx[i], Sy[i], Sz[i],
                dSuX[i], dSuY[i], dSuZ[i],
                dSvX[i], dSvY[i], dSvZ[i]);
      batch.IterCount[i]++;
    }

    // SIMD Newton step
    NewtonStep8_AVX(
      batch.U, batch.V,
      Sx, Sy, Sz,
      dSuX, dSuY, dSuZ,
      dSvX, dSvY, dSvZ,
      batch.RayOrgX, batch.RayOrgY, batch.RayOrgZ,
      batch.RayDirX, batch.RayDirY, batch.RayDirZ,
      du, dv, t, residual);

    // Check convergence and update
    for (int i = 0; i < 8; ++i)
    {
      if (!(activeMask & (1u << i))) continue;

      // Track best result
      if (residual[i] < bestResidual[i])
      {
        bestResidual[i] = residual[i];
        bestU[i] = batch.U[i];
        bestV[i] = batch.V[i];
        bestT[i] = t[i];
        bestPntX[i] = Sx[i];
        bestPntY[i] = Sy[i];
        bestPntZ[i] = Sz[i];
      }

      // Check full convergence
      if (residual[i] < config.Tolerance)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Relaxed early exit after 2 iterations
      if (iter >= 2 && residual[i] < config.RelaxedTol)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = batch.U[i];
        batch.OutV[i] = batch.V[i];
        batch.OutT[i] = t[i];
        batch.OutPntX[i] = Sx[i];
        batch.OutPntY[i] = Sy[i];
        batch.OutPntZ[i] = Sz[i];
        activeMask &= ~(1u << i);
        convergedCount++;
        continue;
      }

      // Stall detection
      if (iter >= 1 && residual[i] > prevResidual[i] * config.StallFactor)
      {
        if (bestResidual[i] < config.RelaxedTol)
        {
          batch.Converged[i] = 1;
          batch.OutU[i] = bestU[i];
          batch.OutV[i] = bestV[i];
          batch.OutT[i] = bestT[i];
          batch.OutPntX[i] = bestPntX[i];
          batch.OutPntY[i] = bestPntY[i];
          batch.OutPntZ[i] = bestPntZ[i];
          convergedCount++;
        }
        activeMask &= ~(1u << i);
        continue;
      }

      prevResidual[i] = residual[i];

      // Update UV with clamping
      batch.U[i] += du[i];
      batch.V[i] += dv[i];
      batch.U[i] = std::max(batch.UMin[i], std::min(batch.UMax[i], batch.U[i]));
      batch.V[i] = std::max(batch.VMin[i], std::min(batch.VMax[i], batch.V[i]));
    }
  }

  // Handle remaining active slots
  for (int i = 0; i < 8; ++i)
  {
    if (activeMask & (1u << i))
    {
      if (bestResidual[i] < config.RelaxedTol * 10.0f)
      {
        batch.Converged[i] = 1;
        batch.OutU[i] = bestU[i];
        batch.OutV[i] = bestV[i];
        batch.OutT[i] = bestT[i];
        batch.OutPntX[i] = bestPntX[i];
        batch.OutPntY[i] = bestPntY[i];
        batch.OutPntZ[i] = bestPntZ[i];
        convergedCount++;
      }
    }
  }

  if (convergedCount == batch.ValidCount)
    return NewtonBatchStatus::AllConverged;
  else if (convergedCount > 0)
    return NewtonBatchStatus::PartialConverge;
  else
    return NewtonBatchStatus::NoneConverged;
}

#endif // OCCT_RT_HAS_AVX

#endif // _BRepIntCurveSurface_RayPacket_HeaderFile
