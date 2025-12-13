# OCCT-RT: High-Performance BVH Raytracer for OpenCASCADE

[![CI](https://github.com/PozzettiAndrea/OCCT-RT/actions/workflows/ci.yml/badge.svg)](https://github.com/PozzettiAndrea/OCCT-RT/actions/workflows/ci.yml)

A BVH-accelerated ray-surface intersection extension for [OpenCASCADE Technology (OCCT)](https://www.opencascade.com/).

[**View Benchmark Results**](https://pozzettiandrea.github.io/OCCT-RT/dev/bench/)

## Features

- **High Performance**: 3.96M rays/sec on 8-core systems using BVH acceleration
- **Multiple Backends**:
  - OCCT's built-in BVH (default, no extra dependencies)
  - Intel Embree with SIMD support (optional, SSE4/AVX)
- **OpenMP Parallelization**: Automatic multi-threaded batch ray processing
- **Newton Refinement**: Exact surface intersection from tessellation-based BVH
- **Curvature Computation**: Gaussian, Mean, Principal curvatures at hit points

## Building

### Prerequisites

- CMake 3.18+
- C++17 compiler
- OpenCASCADE 7.8+ (installed via conda or system package)
- OpenMP (optional, for parallelization)
- Embree 3.x (optional, for SIMD acceleration)

### Install OCCT via Conda (Recommended)

```bash
conda create -n occt-rt python=3.11
conda activate occt-rt
conda install -c conda-forge occt
```

### Clone and Build

```bash
git clone https://github.com/PozzettiAndrea/OCCT-RT.git
cd OCCT-RT
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_RAYTRACER_TEST` | ON | Build the test executable |
| `OCCT_RT_USE_OPENMP` | ON | Enable OpenMP parallelization |
| `OCCT_RT_USE_EMBREE` | ON | Enable Embree backend |

## Usage

### C++ API

```cpp
#include <BRepIntCurveSurface_InterBVH.hxx>
#include <BRepMesh_IncrementalMesh.hxx>

// Tessellate shape first
BRepMesh_IncrementalMesh mesh(shape, 0.1);

// Create raytracer
BRepIntCurveSurface_InterBVH raytracer;
raytracer.SetBackend(BRepIntCurveSurface_BVHBackend::OCCT_BVH);
raytracer.SetUseOpenMP(true);
raytracer.Load(shape, 0.001, 0.1);

// Cast single ray
gp_Lin ray(gp_Pnt(0, 0, 100), gp_Dir(0, 0, -1));
raytracer.Perform(ray);

if (raytracer.IsDone() && raytracer.NbPnt() > 0) {
    gp_Pnt hitPoint = raytracer.Pnt(1);
    gp_Dir normal = raytracer.Normal(1);
    // ...
}
```

### Batch Processing

```cpp
NCollection_Array1<gp_Lin> rays(1, 1000);
// ... fill rays ...

NCollection_Array1<BRepIntCurveSurface_HitResult> results;
raytracer.PerformBatch(rays, results);
```

## Python Bindings

See [occt-rt-python](https://github.com/PozzettiAndrea/occt-rt-python) for Python bindings.

## Architecture

OCCT-RT builds as a standalone library (`libOCCT_RT.so`) that links against an installed OpenCASCADE.

```
Your Application
    └── links → libOCCT_RT.so (this library)
                    └── links → OCCT (TKTopAlgo, TKBRep, TKMath, etc.)
```

## License

LGPL-2.1 with OCCT Exception (same as OpenCASCADE).

## Author

Andrea Pozzetti (with Claude Code assistance)
