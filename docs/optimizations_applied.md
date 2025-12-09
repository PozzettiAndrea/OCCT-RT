# OCCT-RT Optimizations Applied

This document tracks all performance optimizations that have been implemented in the OCCT-RT raytracer.

---

## 1. Embree BVH Backend
**Date:** Early development
**Impact:** ~3-5x faster BVH traversal vs OCCT BVH

Replaced OCCT's built-in BVH with Intel Embree for ray-triangle intersection:
- Embree scalar, SIMD4, and SIMD8 backends supported
- Automatic selection based on hardware capabilities
- Files: `BRepIntCurveSurface_InterBVH.cxx`

---

## 2. SIMD Newton Refinement (NEON/SSE/AVX)
**Date:** Recent
**Impact:** ~2x faster Newton iteration

Vectorized Newton-Raphson refinement for ray-surface intersection:
- 4-wide NEON (ARM) and SSE (x86)
- 8-wide AVX for x86-64
- Batch processing of 4/8 rays simultaneously
- Files: `BRepIntCurveSurface_RayPacket.hxx`

---

## 3. Analytic Quadric Intersection
**Date:** Recent
**Impact:** 1.8-2.2x faster for elementary surfaces

Direct analytic ray-surface intersection (bypasses Newton entirely):
- **Plane:** Single division (t = dot product)
- **Sphere:** Quadratic formula (2 solutions)
- **Cylinder:** Quadratic formula + height bounds
- **Cone:** Quadratic formula + apex handling
- **Torus:** Ferrari's quartic solver (4 solutions)

Performance results:
| Surface | Before | After | Speedup |
|---------|--------|-------|---------|
| Sphere | 2.5M rays/sec | 5.4M rays/sec | **2.2x** |
| Cone | 3.2M rays/sec | 6.1M rays/sec | **1.9x** |
| Torus | 3.7M rays/sec | 6.7M rays/sec | **1.8x** |

Files: `BRepIntCurveSurface_SIMDSurface.hxx`

---

## 4. Custom SIMD B-spline D1 Evaluation
**Date:** 2024-12-09
**Impact:** 2.7x faster Newton phase, 2.2x faster total

Replaced OCCT's `BSplSLib_Cache::D1()` with custom SIMD implementation using pre-computed Bézier patches:

### What was slow:
- OCCT D1() goes through virtual dispatch
- Rebuilds cache when UV crosses knot spans
- Evaluates Bernstein basis serially

### What we did:
1. **Pre-compute Bézier patches** at load time via `GeomConvert_BSplineSurfaceToBezierSurface`
2. **Store control points** in SoA layout (separate X, Y, Z float arrays)
3. **SIMD Bernstein evaluation** - `ComputeBernsteinCubic4_NEON/SSE()` computes 4 basis values + derivatives in parallel
4. **4-wide patch evaluation** - `BezierD1_NEON4/SSE4()` evaluates surface point + derivatives for 4 (u,v) pairs simultaneously
5. **Binary search patch lookup** - `FindPatchIndex()` finds containing patch in O(log n)

### Performance results (B-spline surface, 1024x1024):
| Metric | Before | After | Improvement |
|--------|--------|-------|-------------|
| Phase 2 (Newton + normals) | 531ms | 194ms | **2.7x faster** |
| Total render time | 0.63s | 0.29s | **2.2x faster** |
| Throughput | 1.67M rays/sec | 3.6M rays/sec | **2.2x** |

### Files modified:
- `BRepIntCurveSurface_SIMDSurface.hxx`: Added `BezierD1_NEON4()`, `BezierD1_SSE4()`, `ComputeBernsteinCubic4_NEON/SSE()`, `FindPatchIndex()`
- `BRepIntCurveSurface_InterBVH.cxx`: Integrated custom D1 into `ProcessNewtonBatch4_NEON()`, disabled slower Bézier clipping path

---

## 5. BSplSLib_Cache Pre-computation
**Date:** Recent
**Impact:** Reduced cache rebuilds during Newton iteration

Pre-compute flat knot arrays and weights once per B-spline surface:
- Avoid repeated knot vector construction
- Cache reused across Newton iterations
- Files: `BRepIntCurveSurface_InterBVH.cxx` (struct `BSplineCacheData`)

---

## 6. Two-Phase Ray Processing
**Date:** Recent
**Impact:** Better cache locality, reduced thread contention

Separated ray processing into two phases:
1. **Phase 1:** BVH traversal (triangle hits) - parallel, embarrassingly parallel
2. **Phase 2:** Newton refinement + normal computation - grouped by face for cache locality

Files: `BRepIntCurveSurface_InterBVH.cxx` (`PerformBatch`)

---

## 7. SIMD Surface D1/D2 for Elementary Surfaces
**Date:** Recent
**Impact:** Faster normal/curvature computation

Vectorized D1 and D2 evaluation for torus, cylinder, sphere:
- `TorusD1_NEON4()`, `TorusD2_NEON4()`
- `CylinderD1_NEON4()`, `SphereD1_NEON4()`
- Process 4 (u,v) points simultaneously

Files: `BRepIntCurveSurface_SIMDSurface.hxx`

---

## Summary: Current Performance

| Surface Type | Throughput | Notes |
|--------------|------------|-------|
| Plane | ~10M+ rays/sec | Analytic intersection |
| Sphere | ~5.4M rays/sec | Analytic intersection |
| Cylinder | ~5-6M rays/sec | Analytic intersection |
| Cone | ~6.1M rays/sec | Analytic intersection |
| Torus | ~6.7M rays/sec | Analytic (Ferrari quartic) |
| B-spline (bicubic) | **3.6M rays/sec** | Custom SIMD D1 |
| Many tori (complex) | ~5M rays/sec | Mixed analytic |
