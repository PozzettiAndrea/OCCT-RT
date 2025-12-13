window.BENCHMARK_DATA = {
  "lastUpdate": 1765651384806,
  "repoUrl": "https://github.com/PozzettiAndrea/OCCT-RT",
  "entries": {
    "Benchmark": [
      {
        "commit": {
          "author": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "committer": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "distinct": true,
          "id": "41ca429b9b358644e101579c23528a72bfff532e",
          "message": "Debug Windows: find and copy OCCT_RT.dll\n\nThe raytracer can't find OCCT_RT.dll (our library). Add debugging\nto find where CMake places it in the build directory, then copy it\nto build/bin along with other DLLs.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:49:01Z",
          "tree_id": "c0207d6ff32f25e48c4621d6c576cc9af3c8a675",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/41ca429b9b358644e101579c23528a72bfff532e"
        },
        "date": 1765645001280,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 545010,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1834.83ms"
          },
          {
            "name": "box",
            "value": 6339401,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 157.74ms"
          },
          {
            "name": "bspline_surface",
            "value": 2212339,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 452.01ms"
          },
          {
            "name": "cone",
            "value": 2579953,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 387.60ms"
          },
          {
            "name": "cylinder",
            "value": 3933035,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 254.26ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1610042,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 621.10ms"
          },
          {
            "name": "mr",
            "value": 3377984,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 296.03ms"
          },
          {
            "name": "simple_L021",
            "value": 2152970,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 464.47ms"
          },
          {
            "name": "sphere",
            "value": 1850650,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 540.35ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2047671,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 488.36ms"
          },
          {
            "name": "torus",
            "value": 2063381,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 484.64ms"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "committer": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "distinct": true,
          "id": "f328d144d970736e029332a469674c798bb8cbe1",
          "message": "Add Windows Embree/SIMD4/SIMD8 benchmark configurations\n\n- Add win-x64-embree, win-x64-simd4, win-x64-simd8 jobs to CI matrix\n- Include embree*.dll in Windows DLL copy step\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T17:01:27Z",
          "tree_id": "2d2257f28d1e0b2b1f826e44f318d75f5e0e73af",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/f328d144d970736e029332a469674c798bb8cbe1"
        },
        "date": 1765645926799,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 560379,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1784.51ms"
          },
          {
            "name": "box",
            "value": 6845175,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 146.09ms"
          },
          {
            "name": "bspline_surface",
            "value": 2194448,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 455.70ms"
          },
          {
            "name": "cone",
            "value": 2750805,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 363.53ms"
          },
          {
            "name": "cylinder",
            "value": 4336098,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 230.62ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1656101,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 603.83ms"
          },
          {
            "name": "mr",
            "value": 3421516,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 292.27ms"
          },
          {
            "name": "simple_L021",
            "value": 2195698,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 455.44ms"
          },
          {
            "name": "sphere",
            "value": 1875563,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 533.17ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2036394,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 491.06ms"
          },
          {
            "name": "torus",
            "value": 2075072,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 481.91ms"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "committer": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "distinct": true,
          "id": "5d152d8fd23370d975506ed4e667eb7bdca4c5fc",
          "message": "Add macOS ARM64 Embree/SIMD4/SIMD8 benchmark configurations\n\n- Embree supports ARM64 since v3.13, conda-forge has osx-arm64 packages\n- Add macos-arm64-embree, macos-arm64-simd4, macos-arm64-simd8 jobs\n- Update macOS install step to include Embree when needed\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T17:42:58Z",
          "tree_id": "f092267c80309d0261db9099979e779557b5cbb5",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/5d152d8fd23370d975506ed4e667eb7bdca4c5fc"
        },
        "date": 1765648242335,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 544270,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1837.32ms"
          },
          {
            "name": "box",
            "value": 6965583,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 143.56ms"
          },
          {
            "name": "bspline_surface",
            "value": 2443842,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 409.19ms"
          },
          {
            "name": "cone",
            "value": 2786892,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 358.82ms"
          },
          {
            "name": "cylinder",
            "value": 4123691,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 242.50ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1707409,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 585.68ms"
          },
          {
            "name": "mr",
            "value": 3540185,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 282.47ms"
          },
          {
            "name": "simple_L021",
            "value": 2240802,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 446.27ms"
          },
          {
            "name": "sphere",
            "value": 1948303,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 513.27ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2099689,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 476.26ms"
          },
          {
            "name": "torus",
            "value": 2180135,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 458.69ms"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "committer": {
            "email": "andrea@pozzetti.it",
            "name": "Andrea Pozzetti",
            "username": "PozzettiAndrea"
          },
          "distinct": true,
          "id": "9f028a80159dbb4f510bce14f5f76feaae6063ef",
          "message": "Add interactive GLFW viewer with trackball rotation\n\nFeatures:\n- occt-rt-viewer executable for BREP/STEP/IGES files\n- Perspective camera with quaternion-based trackball (no gimbal lock)\n- ParaView-style navigation: left-drag rotate, scroll zoom, middle-drag pan\n- Lambertian shading from surface normals\n- Low-res preview during interaction for smooth rotation\n- Connected component validation (errors if not single component)\n- GitHub release workflow for cross-platform binaries\n\nNew files:\n- src/Viewer/Viewer_Camera.hxx/cxx - Perspective camera + trackball\n- src/Viewer/Viewer_CADLoader.hxx/cxx - Unified CAD loader\n- tools/viewer.cxx - GLFW render loop\n- .github/workflows/release.yml - Release automation\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T18:34:57Z",
          "tree_id": "485348c84ad7039b7b65d5a6adcb86718c1e91e7",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/9f028a80159dbb4f510bce14f5f76feaae6063ef"
        },
        "date": 1765651379699,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 622877,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1605.45ms"
          },
          {
            "name": "box",
            "value": 7053012,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 141.78ms"
          },
          {
            "name": "bspline_surface",
            "value": 2446264,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 408.79ms"
          },
          {
            "name": "cone",
            "value": 2799856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 357.16ms"
          },
          {
            "name": "cylinder",
            "value": 4398592,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 227.35ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1701271,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 587.80ms"
          },
          {
            "name": "mr",
            "value": 3521887,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 283.94ms"
          },
          {
            "name": "simple_L021",
            "value": 2251169,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 444.21ms"
          },
          {
            "name": "sphere",
            "value": 1962516,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 509.55ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2088927,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 478.71ms"
          },
          {
            "name": "torus",
            "value": 2176166,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 459.52ms"
          }
        ]
      }
    ]
  }
}