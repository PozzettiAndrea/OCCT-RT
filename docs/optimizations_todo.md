# OCCT-RT Optimizations To Do

This document tracks potential performance optimizations that have been identified but not yet implemented.

---

## Quick Wins (Low effort, good impact)

### 1. Newton Configuration Tuning
**Impact:** +10-20% | **Effort:** 0.5 days | **Priority:** HIGH

Current defaults are conservative for CAD precision:
- Tolerance: 1e-7
- Max iterations: 10

For rendering/visualization, can relax to:
- Tolerance: 1e-4 or 1e-5
- Max iterations: 5

Most rays converge in 2-3 iterations anyway. Add configurable profiles:
```cpp
enum NewtonProfile { Precise, Balanced, Fast };
void SetNewtonProfile(NewtonProfile profile);
```

**Validation needed:** Ensure visual quality doesn't degrade.

**Files:** `BRepIntCurveSurface_InterBVH.cxx`

---

### 2. Planar Face Normal Precomputation
**Impact:** +3-5% | **Effort:** 0.5 days | **Priority:** HIGH

Currently computing D1() for planar faces to get normals, but the normal is constant per face.

**Current code:**
```cpp
if (isPlanar) {
  aSurface.D1(initU, initV, normPnt, dSdu, dSdv);  // REDUNDANT!
  gp_Vec normalVec = dSdu.Crossed(dSdv);
}
```

**Fix:**
- Precompute normals at load time (where face orientation is already cached)
- Store in `std::vector<gp_Dir> myPlanarFaceNormals`
- Skip D1 call entirely for planar hits

**Files:** `BRepIntCurveSurface_InterBVH.cxx`

---

## Medium Effort Optimizations

### 3. Thread-Local Surface Copy Optimization
**Impact:** +5-8% | **Effort:** 1 day | **Priority:** MEDIUM

`ShallowCopy()` creates thread-local surface adaptors per parallel region:
```cpp
surfaces[i] = mySurfaceAdaptors[i]->ShallowCopy();
```

ShallowCopy() involves Handle reference counting and internal state duplication.

**Questions to investigate:**
- Are surfaces only read, not modified? → Use const references
- Can we cache ShallowCopy results per thread?
- Would `#pragma omp threadprivate` help?

**Files:** `BRepIntCurveSurface_InterBVH.cxx`

---

### 4. B-Spline Cache Pooling
**Impact:** +3-5% | **Effort:** 2 days | **Priority:** MEDIUM

When UV crosses knot spans during Newton iteration, BSplSLib_Cache is reallocated:
```cpp
if (uSpan != lastUSpan || vSpan != lastVSpan) {
  theCache = new BSplSLib_Cache(...);  // ALLOCATION in tight loop!
}
```

**Fix:**
- Pre-allocate cache pool (2-3 caches per thread) at load time
- LRU eviction for span transitions
- Avoid new/delete in Newton loop

**Files:** `BRepIntCurveSurface_InterBVH.cxx`

---

### 5. OpenMP Schedule Tuning
**Impact:** +2-4% | **Effort:** 0.5 days | **Priority:** LOW

Current: `#pragma omp for schedule(dynamic, 64)`

Dynamic scheduling with chunk size 64 assumes uniform per-ray cost, but:
- Planar face hits are ~10x faster than curved hits
- Some threads finish early, wait for Newton-heavy threads

**Try:**
- `schedule(guided, 32)` - adaptive chunk sizing
- Measure if dynamic overhead > load imbalance cost

**Files:** `BRepIntCurveSurface_InterBVH.cxx`

---

## Architecture Improvements (Higher effort)

### 6. AVX-512 / 16-Wide SIMD
**Impact:** +10-15% | **Effort:** 3-4 days | **Priority:** MEDIUM (x86 only)

Current Newton batches are 4-wide (NEON/SSE) or 8-wide (AVX). Modern Intel CPUs support AVX-512:
- 16-wide batches = 2x fewer iteration loop overheads
- 125k packets @ 8-wide → 62.5k packets @ 16-wide

**Requires:**
- `NewtonIterateBatch16_AVX512()`
- `RayBox16_AVX512()`
- `BezierD1_AVX512_16()` for B-splines
- Conditional compile: `#if defined(__AVX512F__)`

**Files:** `BRepIntCurveSurface_RayPacket.hxx`, `BRepIntCurveSurface_SIMDSurface.hxx`

---

### 7. BVH Coherency Analysis
**Impact:** +5-8% (or 0%) | **Effort:** 2 days | **Priority:** LOW

Ray sorting for coherent traversal exists but has overhead:
- Morton code computation (bit interleaving)
- Sorting: O(n log n)

**Questions:**
- Does coherent tracing actually help for B-spline surfaces?
- Can we cache ray sorting when rays are stable (same camera)?
- Adaptive sorting based on ray direction variance?

**Measure first** before optimizing.

**Files:** `BRepIntCurveSurface_InterBVH.cxx`, `BRepIntCurveSurface_RayPacket.hxx`

---

### 8. Non-Bicubic B-spline Support
**Impact:** Correctness | **Effort:** 1-2 days | **Priority:** LOW

Current custom SIMD D1 only supports bicubic (degree 3x3) B-splines. For other degrees:
- Falls back to OCCT BSplSLib_Cache (slower)
- Could extend Bernstein evaluation to arbitrary degree

**Files:** `BRepIntCurveSurface_SIMDSurface.hxx`

---

### 9. GPU Acceleration (Future)
**Impact:** 10-100x | **Effort:** Weeks | **Priority:** FUTURE

For massive ray counts, GPU acceleration via:
- CUDA/OptiX (NVIDIA)
- HIP (AMD)
- Metal (Apple)

Would require significant architecture changes but could achieve 100M+ rays/sec.

---

## Summary Table

| # | Optimization | Impact | Effort | Priority |
|---|--------------|--------|--------|----------|
| 1 | Newton config tuning | +10-20% | 0.5 days | **HIGH** |
| 2 | Planar normal precompute | +3-5% | 0.5 days | **HIGH** |
| 3 | Thread-local surface opt | +5-8% | 1 day | MEDIUM |
| 4 | B-spline cache pool | +3-5% | 2 days | MEDIUM |
| 5 | OpenMP schedule tuning | +2-4% | 0.5 days | LOW |
| 6 | AVX-512 support | +10-15% | 3-4 days | MEDIUM |
| 7 | BVH coherency analysis | +5-8%? | 2 days | LOW |
| 8 | Non-bicubic B-splines | Correctness | 1-2 days | LOW |
| 9 | GPU acceleration | 10-100x | Weeks | FUTURE |

---

## Estimated Performance Targets

| Optimization Path | Gain | Target Throughput |
|-------------------|------|-------------------|
| Current baseline | - | 3.6M rays/sec |
| + Quick wins (#1, #2) | +13-25% | **4.0-4.5M rays/sec** |
| + Medium effort (#3, #4) | +8-13% | **4.5-5.0M rays/sec** |
| + AVX-512 (#6) | +10-15% | **5.0-5.5M rays/sec** |
| + All optimizations | +35-55% | **5-6M rays/sec** |
