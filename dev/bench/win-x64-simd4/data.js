window.BENCHMARK_DATA = {
  "lastUpdate": 1765651546218,
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
          "id": "f328d144d970736e029332a469674c798bb8cbe1",
          "message": "Add Windows Embree/SIMD4/SIMD8 benchmark configurations\n\n- Add win-x64-embree, win-x64-simd4, win-x64-simd8 jobs to CI matrix\n- Include embree*.dll in Windows DLL copy step\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T17:01:27Z",
          "tree_id": "2d2257f28d1e0b2b1f826e44f318d75f5e0e73af",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/f328d144d970736e029332a469674c798bb8cbe1"
        },
        "date": 1765645759797,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 2934277,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 340.80ms"
          },
          {
            "name": "box",
            "value": 8902579,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 112.33ms"
          },
          {
            "name": "bspline_surface",
            "value": 2392237,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 418.02ms"
          },
          {
            "name": "cone",
            "value": 3628048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 275.63ms"
          },
          {
            "name": "cylinder",
            "value": 8940218,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 111.85ms"
          },
          {
            "name": "io1-ca-214",
            "value": 176316,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 5671.64ms"
          },
          {
            "name": "mr",
            "value": 4436532,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 225.40ms"
          },
          {
            "name": "simple_L021",
            "value": 4487484,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 222.84ms"
          },
          {
            "name": "sphere",
            "value": 2209110,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 452.67ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 1781146,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 561.44ms"
          },
          {
            "name": "torus",
            "value": 3003753,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 332.92ms"
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
        "date": 1765648319348,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 3147532,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 317.71ms"
          },
          {
            "name": "box",
            "value": 10893222,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 91.80ms"
          },
          {
            "name": "bspline_surface",
            "value": 2789740,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 358.46ms"
          },
          {
            "name": "cone",
            "value": 4072523,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 245.55ms"
          },
          {
            "name": "cylinder",
            "value": 10457965,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 95.62ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2181563,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 458.39ms"
          },
          {
            "name": "mr",
            "value": 7325476,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 136.51ms"
          },
          {
            "name": "simple_L021",
            "value": 6566010,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 152.30ms"
          },
          {
            "name": "sphere",
            "value": 3405061,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 293.68ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2733274,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 365.86ms"
          },
          {
            "name": "torus",
            "value": 4720675,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 211.83ms"
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
        "date": 1765651410471,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 4465111,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 223.96ms"
          },
          {
            "name": "box",
            "value": 10207415,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 97.97ms"
          },
          {
            "name": "bspline_surface",
            "value": 2433593,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 410.92ms"
          },
          {
            "name": "cone",
            "value": 5073070,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 197.12ms"
          },
          {
            "name": "cylinder",
            "value": 12477914,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.14ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2650992,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 377.22ms"
          },
          {
            "name": "mr",
            "value": 8976749,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 111.40ms"
          },
          {
            "name": "simple_L021",
            "value": 7930667,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 126.09ms"
          },
          {
            "name": "sphere",
            "value": 3740675,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 267.33ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2496659,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 400.54ms"
          },
          {
            "name": "torus",
            "value": 5775666,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 173.14ms"
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
          "id": "b93657ebb7e646875f7de8f8f30aa9351c9e5798",
          "message": "Fix YAML heredoc syntax in release workflow\n\nReplace heredocs with printf/echo to avoid YAML parsing issues.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T18:37:02Z",
          "tree_id": "87240cc2c1a6c9fc78abcd413e8b478e589c60df",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/b93657ebb7e646875f7de8f8f30aa9351c9e5798"
        },
        "date": 1765651517805,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 3916735,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 255.31ms"
          },
          {
            "name": "box",
            "value": 10355944,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 96.56ms"
          },
          {
            "name": "bspline_surface",
            "value": 2689115,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 371.87ms"
          },
          {
            "name": "cone",
            "value": 3903163,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 256.20ms"
          },
          {
            "name": "cylinder",
            "value": 10780439,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 92.76ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2060398,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 485.34ms"
          },
          {
            "name": "mr",
            "value": 4272736,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 234.04ms"
          },
          {
            "name": "simple_L021",
            "value": 6197407,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 161.36ms"
          },
          {
            "name": "sphere",
            "value": 4124666,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 242.44ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2393497,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 417.80ms"
          },
          {
            "name": "torus",
            "value": 4463423,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 224.04ms"
          }
        ]
      }
    ]
  }
}