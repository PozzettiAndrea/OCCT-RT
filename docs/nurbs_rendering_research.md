# Research: Analytical NURBS Rendering Techniques

## Goal
Research alternative rendering approaches for CAD surfaces - specifically analytical rendering
without tessellation, using either rasterization or raytracing paradigms.

## Key Finding: Rasterization vs Raytracing Converge

For parametric surfaces (NURBS, Bezier), both approaches reduce to the same core problem:

> **Given a screen pixel, find the surface parameters (u,v) where the surface is visible.**

| Approach | How it works | Core computation |
|----------|--------------|------------------|
| **Raytracing** | Cast ray through pixel → intersect surface | Newton iteration for (u,v) |
| **Rasterization** | Project surface bounds → fill pixels → solve | Newton iteration for (u,v) |

**Your current raytracer already does analytical NURBS intersection!** The Newton refinement
step finds exact (u,v) on the true surface, not just triangle intersection.

---

## Rendering Techniques Researched

### 1. Current OCCT-RT Approach (Implemented)
- BVH-accelerated ray-triangle intersection
- Newton refinement to exact NURBS surface
- CPU-based, OpenMP parallelized
- **Status**: Working, used for benchmarks

### 2. Catmull's Recursive Subdivision (1974)
Classic algorithm for parametric surfaces:
```
render_patch(patch):
    if patch projects to < 1 pixel:
        fill_pixel(patch.center)
    else:
        subdivide into 4 sub-patches
        recurse on each
```
- **Pros**: No triangles, smooth silhouettes, compact
- **Cons**: May oversample, limited parallelism
- **Source**: [Rendering Cubic Bezier Patches](https://web.cs.wpi.edu/~matt/courses/cs563/talks/surface/bez_surf.html)

### 3. Point Inversion + Newton (Modern CPU)
For each pixel in projected surface bounds:
1. Newton iteration to find (u,v) where S(u,v) projects to pixel
2. If converges within patch domain → visible hit
3. Z-buffer for depth sorting

- **Performance**: 550K-7M points/sec on CPU
- **Source**: [Practical inverse evaluation of NURBS](https://www.researchgate.net/publication/314146025)

### 4. OCCT V3d Built-in Rendering
OCCT has complete GPU visualization system:
- `TKV3d` / `TKOpenGl` modules
- `V3d_View::ToPixMap()` for headless rendering
- Supports both rasterization AND GPU raytracing
- **But**: Works on tessellated geometry, not analytical NURBS

### 5. ETER - Elastic Tessellation (SIGGRAPH 2023)
- GPU Tensor Core accelerated NURBS evaluation
- 2.9-16.2x faster than hardware tessellation
- 3.7M patches at 30 FPS
- **Source**: [ETER Paper](https://dl.acm.org/doi/10.1145/3592419)

### 6. PaRas - Parametric Surface Rasterizer (SIGGRAPH 2025)
- Direct parametric surface rasterization
- Newton-type iteration on GPU per pixel
- No tessellation, pixel-accurate
- **Source**: [PaRas Paper](https://dl.acm.org/doi/10.1145/3721238.3730658)

### 7. GPU Fragment Shader Ray Casting
- Render bounding hulls as geometry
- Fragment shader does Newton iteration per pixel
- True analytical rendering on GPU
- **Source**: [Ray Casting of Trimmed NURBS on GPU](https://ieeexplore.ieee.org/document/4061557/)

---

## OCCT Visualization Capabilities

OCCT has two rendering modes in V3d:
```cpp
enum Graphic3d_RenderingMode {
    Graphic3d_RM_RASTERIZATION,  // Fast OpenGL triangles
    Graphic3d_RM_RAYTRACING      // GPU GLSL raytracing
};
```

Both work on **tessellated geometry**, not analytical NURBS. The tessellation is done via
`BRepMesh_IncrementalMesh` before rendering.

Headless rendering API:
```cpp
V3d_View::ToPixMap(Image_PixMap& image, V3d_ImageDumpOptions& options)
```

---

## Key Papers & Sources

1. [Point inversion for NURBS - Control Polygon](https://www.sciencedirect.com/science/article/abs/pii/S0167839603000219)
2. [Multipoint Inversion on Parametric Surfaces](https://onlinelibrary.wiley.com/doi/10.1155/2019/3790762)
3. [ETER: Elastic Tessellation for NURBS](https://dl.acm.org/doi/10.1145/3592419)
4. [PaRas: Rasterizer for Parametric Surfaces](https://dl.acm.org/doi/10.1145/3721238.3730658)
5. [Direct trimming of NURBS on GPU](https://www.researchgate.net/publication/216813514)
6. [Ray Casting of Trimmed NURBS on GPU](https://ieeexplore.ieee.org/document/4061557/)
7. [Optimizing Ray Tracing of Trimmed NURBS](https://onlinelibrary.wiley.com/doi/abs/10.1111/cgf.14410)
8. [Interactive rendering of NURBS surfaces](https://core.ac.uk/download/pdf/80522586.pdf)

---

## Performance: Raytracing vs Rasterization for NURBS

### Would Rasterization Be Faster?

**Potentially yes, for sparse scenes:**

| Approach | Empty Space | Dense Scenes | Core Cost |
|----------|-------------|--------------|-----------|
| **Raytracing** | Tests all pixels (wastes rays) | Efficient | Newton iteration |
| **Rasterization** | Only fills covered pixels | Similar | Newton iteration |

Both end up doing Newton iteration per visible pixel - that's the expensive part.
The difference is pixel selection strategy.

### Published Performance Numbers

| Method | Performance | Source |
|--------|-------------|--------|
| **CPU NURBS Ray Tracing** (2006) | ~1-3 FPS for complex models | [Direct and Fast Ray Tracing](https://ieeexplore.ieee.org/document/4061558/) |
| **GPU NURBS Ray Tracing** (2021) | 10s-100s million rays/sec | [Optimizing Ray Tracing Trimmed NURBS](https://onlinelibrary.wiley.com/doi/abs/10.1111/cgf.14410) |
| **ETER** (2023) | 3.7M patches @ 30 FPS | [ETER Paper](https://dl.acm.org/doi/10.1145/3592419) |
| **PaRas** (2025) | "Higher efficiency than tessellation" | [PaRas Paper](https://dl.acm.org/doi/10.1145/3721238.3730658) |

### PaRas Key Insight

> "formulate the rasterization of parametric surfaces as a **point inversion problem**,
> employing a **Newton-type iteration** on the GPU to compute precise solutions"

Same Newton iteration as raytracing, but:
- Project patch bounds to screen first
- Only iterate on covered pixels
- Avoids testing empty space

---

## Conclusion

Your current CPU raytracer with Newton refinement **is** analytical NURBS rendering. The
"rasterization" alternative would use the same Newton iteration but select pixels differently
(project-then-fill vs cast-rays-everywhere).

**Could rasterization be faster?** Yes, especially for:
- Sparse scenes (few surfaces, lots of background)
- Wide views (surfaces occupy small screen area)
- The savings come from NOT processing empty pixels

**When raytracing is fine:**
- Dense scenes (surfaces fill screen)
- Embree SIMD packets give good cache coherence
- BVH early termination helps with occlusion

For faster renders without GPU:
- Rasterization-style approach (project, then Newton only on covered pixels)
- Better Newton initial guesses (control polygon analysis)
- Use SIMD (Embree) for faster ray-triangle intersection

For GPU acceleration (future):
- OCCT V3d for quick tessellated renders
- Custom fragment shader ray casting for analytical quality
- PaRas/ETER-style approaches for state-of-the-art

---

## OCCT-RT Newton Implementation Analysis

### Algorithm Overview

**Location:** `src/BRepIntCurveSurface/BRepIntCurveSurface_InterBVH.cxx` (lines 166-621)

Your implementation is a **Gauss-Newton variant** with pragmatic optimizations for throughput.

### Key Design Decisions

| Aspect | Your Implementation | Classic Papers |
|--------|---------------------|----------------|
| **Initial guess** | Barycentric from triangle mesh | Grid search / control polygon |
| **Method** | Gauss-Newton (J^T J) | Same or full Newton |
| **Derivatives** | D1 only | D1 (Gauss) or D1+D2 (Newton) |
| **Iterations** | 2-3 typical (early exit) | 5-10 typical |
| **Tolerance** | Multi-level: 1e-7 → 1e-5 → 1e-6 | Simple threshold |
| **Damping** | Hard step-size clip at 0.5 | Levenberg-Marquardt or none |

### Why Your Implementation is Fast

1. **Excellent initial guess**: Triangle barycentric interpolation gives UV within ~0.01 of true solution
2. **Aggressive early exit**: Accepts 100x relaxed tolerance after 2 iterations
3. **Best-result tracking**: Never loses progress, returns best even if stalled
4. **Stall detection**: Exits if <20% progress per iteration
5. **Analytic shortcuts**: Plane, sphere, cylinder, cone, torus skip Newton entirely

### Performance Numbers (from benchmarks)

```
Sphere:   5.4M rays/sec  (analytic)
Cylinder: 5-6M rays/sec  (analytic)
Cone:     6.1M rays/sec  (analytic)
Torus:    6.7M rays/sec  (analytic quartic)
B-spline: 3.6M rays/sec  (Newton required)
```

### Jacobian Construction (lines 291-317)

```cpp
// Gauss-Newton: J = [dS/du, dS/dv]^T * [dS/du, dS/dv]
j11 = dSu.Dot(dSu);    // ||dS/du||²
j12 = dSu.Dot(dSv);    // dS/du · dS/dv
j22 = dSv.Dot(dSv);    // ||dS/dv||²

// Right-hand side
b1 = -r.Dot(dSu);      // -residual · dS/du
b2 = -r.Dot(dSv);      // -residual · dS/dv

// 2x2 solve
det = j11*j22 - j12*j21;
du = (j22*b1 - j12*b2) / det;
dv = (j11*b2 - j21*b1) / det;
```

### SIMD Batched Variants

- `ProcessNewtonBatch4_NEON()` - 4 rays on ARM
- `ProcessNewtonBatch4_SSE()` - 4 rays on x86
- `ProcessNewtonBatch8_AVX()` - 8 rays on x86-64
- Custom B-spline D1 evaluation: **2.7x speedup**

### Comparison to PaRas/ETER

| Feature | OCCT-RT | PaRas/ETER |
|---------|---------|------------|
| **Platform** | CPU (SIMD) | GPU (CUDA/GLSL) |
| **Pixel selection** | Raytracing (all pixels) | Rasterization (covered only) |
| **Newton iteration** | Gauss-Newton | Newton-type (similar) |
| **Initial guess** | Triangle barycentric | Patch projection |
| **Parallelism** | Per-ray batches (4-8) | Per-pixel massively parallel |

### Potential Improvements (from research)

1. **Rasterization-style pixel selection**: Project patch bounds, only Newton on covered pixels
2. **Better initial guess**: Control polygon subdivision for hard cases
3. **Bezier clipping**: Already implemented but disabled (`RayBezierClip()`)
4. **GPU port**: Fragment shader Newton (same algorithm, massive parallelism)
