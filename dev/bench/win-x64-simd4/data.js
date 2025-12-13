window.BENCHMARK_DATA = {
  "lastUpdate": 1765668590161,
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
          "id": "21aead782944001b36bee36a52ac4cc20035f71e",
          "message": "Fix remaining YAML multi-line string issues in release workflow\n\nConvert all multi-line strings to single-line printf/PowerShell format.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T18:38:35Z",
          "tree_id": "f7b591fb00e98f686ddd9077e75edd1f24e7fe93",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/21aead782944001b36bee36a52ac4cc20035f71e"
        },
        "date": 1765651642748,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5825920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 171.65ms"
          },
          {
            "name": "box",
            "value": 12375196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.81ms"
          },
          {
            "name": "bspline_surface",
            "value": 3504629,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 285.34ms"
          },
          {
            "name": "cone",
            "value": 5110755,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 195.67ms"
          },
          {
            "name": "cylinder",
            "value": 12488199,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.08ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2710311,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 368.96ms"
          },
          {
            "name": "mr",
            "value": 8707032,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.85ms"
          },
          {
            "name": "simple_L021",
            "value": 7975092,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.39ms"
          },
          {
            "name": "sphere",
            "value": 4303785,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 232.35ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3434594,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 291.16ms"
          },
          {
            "name": "torus",
            "value": 5866204,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.47ms"
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
          "id": "e9f8a693514f1d1ab77c36cf19faa79c327f468a",
          "message": "Add TKXSBASE library for XSControl_Reader symbols\n\nFixes macOS build failure due to missing XSControl_Reader symbols.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T18:47:16Z",
          "tree_id": "9fa1e9da59b64d9e2f687cc5ef5f1a0a7a864618",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/e9f8a693514f1d1ab77c36cf19faa79c327f468a"
        },
        "date": 1765652105367,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5721128,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 174.79ms"
          },
          {
            "name": "box",
            "value": 12778083,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 78.26ms"
          },
          {
            "name": "bspline_surface",
            "value": 3530699,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 283.23ms"
          },
          {
            "name": "cone",
            "value": 4799328,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 208.36ms"
          },
          {
            "name": "cylinder",
            "value": 12457861,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.27ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2695225,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 371.03ms"
          },
          {
            "name": "mr",
            "value": 7689013,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 130.06ms"
          },
          {
            "name": "simple_L021",
            "value": 7685420,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 130.12ms"
          },
          {
            "name": "sphere",
            "value": 4048493,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 247.01ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3245069,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 308.16ms"
          },
          {
            "name": "torus",
            "value": 5621489,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 177.89ms"
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
          "id": "4496fdada6c2fdc81498712e82b673599428ba34",
          "message": "Fix TKXSBase library name case\n\nUse TKXSBase (correct case) instead of TKXSBASE.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T19:08:31Z",
          "tree_id": "5436339ef80590e9dffa5eb89a7325553dcb4ce2",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/4496fdada6c2fdc81498712e82b673599428ba34"
        },
        "date": 1765653488581,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 4408244,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 226.85ms"
          },
          {
            "name": "box",
            "value": 8437537,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 118.52ms"
          },
          {
            "name": "bspline_surface",
            "value": 2578720,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 387.79ms"
          },
          {
            "name": "cone",
            "value": 3724502,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 268.49ms"
          },
          {
            "name": "cylinder",
            "value": 9548153,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 104.73ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2047917,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 488.30ms"
          },
          {
            "name": "mr",
            "value": 8317108,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 120.23ms"
          },
          {
            "name": "simple_L021",
            "value": 6993873,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 142.98ms"
          },
          {
            "name": "sphere",
            "value": 3786832,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 264.07ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3230027,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 309.59ms"
          },
          {
            "name": "torus",
            "value": 5577783,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 179.28ms"
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
          "id": "9575bef07694eb15b6c47fc2a3aef882e449fb72",
          "message": "Add ad-hoc codesigning for macOS releases\n\nSign all dylibs and executables with ad-hoc signature for better\nGatekeeper compatibility. Users will need to approve the app once\non first run (right-click → Open) but won't see the \"damaged\" error.\n\nAlso adds macOS first-run instructions to the bundled README.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T22:19:28Z",
          "tree_id": "6d151b1308432793d2cdedb34c3ab2d36c98fee7",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/9575bef07694eb15b6c47fc2a3aef882e449fb72"
        },
        "date": 1765664859794,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5794998,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 172.56ms"
          },
          {
            "name": "box",
            "value": 12298429,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 81.31ms"
          },
          {
            "name": "bspline_surface",
            "value": 3449669,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 289.88ms"
          },
          {
            "name": "cone",
            "value": 5130513,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 194.91ms"
          },
          {
            "name": "cylinder",
            "value": 12302605,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.28ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2704645,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 369.73ms"
          },
          {
            "name": "mr",
            "value": 8753394,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.24ms"
          },
          {
            "name": "simple_L021",
            "value": 7910885,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 126.41ms"
          },
          {
            "name": "sphere",
            "value": 4290488,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 233.07ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3468900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 288.28ms"
          },
          {
            "name": "torus",
            "value": 5848343,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.99ms"
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
          "id": "3329e5704e7c69e8ea5676f617d9c965f5f9d713",
          "message": "Add CAD face type tracking to raytracer and benchmarks\n\n- Add FaceTypeCounts struct to BRepIntCurveSurface_InterBVH to track\n  face types (plane, cylinder, cone, sphere, torus, bspline, bezier, other)\n- Output face type breakdown in raytracer.cxx after loading\n- Parse face count and types in render_shaded.py for metadata.json\n- Benchmark page updated (gh-pages) to display face information\n\nThis enables better understanding of geometry complexity in benchmark results.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T22:56:36Z",
          "tree_id": "6a069b08e788c56ad41d12b3d6f8d263d440dd6d",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/3329e5704e7c69e8ea5676f617d9c965f5f9d713"
        },
        "date": 1765667084220,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5878508,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 170.11ms"
          },
          {
            "name": "box",
            "value": 12555842,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.64ms"
          },
          {
            "name": "bspline_surface",
            "value": 3544923,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 282.09ms"
          },
          {
            "name": "cone",
            "value": 5160449,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 193.78ms"
          },
          {
            "name": "cylinder",
            "value": 12802671,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.11ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2674644,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 373.88ms"
          },
          {
            "name": "mr",
            "value": 8963023,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 111.57ms"
          },
          {
            "name": "simple_L021",
            "value": 8052314,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 124.19ms"
          },
          {
            "name": "sphere",
            "value": 4268107,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 234.30ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3411066,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 293.16ms"
          },
          {
            "name": "torus",
            "value": 4403983,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 227.07ms"
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
          "id": "968ffaf7fcadf8ccb1d43c4053e33f9b74362514",
          "message": "Add multi-level benchmark support\n\nAdd 4 benchmark levels to measure performance at each computation stage:\n- raw: BVH traversal + triangle intersection only (skip Newton)\n- refined: Above + Newton refinement for exact UV\n- normals: Above + D1 evaluation for surface normals\n- curvature: Above + D2 evaluation for curvature values\n\nChanges:\n- Add SetSkipAllNewton() to InterBVH for raw benchmark mode\n- Add --bench-level flag to raytracer CLI\n- Update parse_benchmark.py to support --bench-level\n- Update render_shaded.py to generate 4 render types (depth, normals, curvature)\n- Update CI workflow to run all 4 benchmark levels\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T23:15:54Z",
          "tree_id": "44cf71390c2cde5b7d1dec27a89e7e4168c9f916",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/968ffaf7fcadf8ccb1d43c4053e33f9b74362514"
        },
        "date": 1765668580991,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 9304489,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 107.48ms"
          },
          {
            "name": "box_raw",
            "value": 12528298,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.82ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8925918,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 112.03ms"
          },
          {
            "name": "cone_raw",
            "value": 11231071,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.04ms"
          },
          {
            "name": "cylinder_raw",
            "value": 13312956,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 75.11ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9593366,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 104.24ms"
          },
          {
            "name": "mr_raw",
            "value": 13338384,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 74.97ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 8563623,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 116.77ms"
          },
          {
            "name": "sphere_raw",
            "value": 10568121,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 94.62ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10736606,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 93.14ms"
          },
          {
            "name": "torus_raw",
            "value": 12284228,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 81.41ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6214368,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 160.92ms"
          },
          {
            "name": "box_refined",
            "value": 12481153,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.12ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3912019,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.62ms"
          },
          {
            "name": "cone_refined",
            "value": 5438196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 183.88ms"
          },
          {
            "name": "cylinder_refined",
            "value": 12951371,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 77.21ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 2598792,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 384.79ms"
          },
          {
            "name": "mr_refined",
            "value": 8302807,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 120.44ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 8184137,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 122.19ms"
          },
          {
            "name": "sphere_refined",
            "value": 4454073,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 224.51ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3579933,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 279.33ms"
          },
          {
            "name": "torus_refined",
            "value": 6200055,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 161.29ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 6179870,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 161.82ms"
          },
          {
            "name": "box_normals",
            "value": 11610950,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 86.13ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 3803317,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 262.93ms"
          },
          {
            "name": "cone_normals",
            "value": 5188280,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 192.74ms"
          },
          {
            "name": "cylinder_normals",
            "value": 12939306,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 77.28ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2941196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 340.00ms"
          },
          {
            "name": "mr_normals",
            "value": 8696643,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.99ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 7498500,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 133.36ms"
          },
          {
            "name": "sphere_normals",
            "value": 4176340,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 239.44ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3416737,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 292.68ms"
          },
          {
            "name": "torus_normals",
            "value": 5700036,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 175.44ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 5591672,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 178.84ms"
          },
          {
            "name": "box_curvature",
            "value": 11811396,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 84.66ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3291755,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 303.79ms"
          },
          {
            "name": "cone_curvature",
            "value": 4956568,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 201.75ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 12258237,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.58ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2597224,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 385.03ms"
          },
          {
            "name": "mr_curvature",
            "value": 5415657,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 184.65ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 6947566,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 143.94ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4242230,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 235.73ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3260355,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 306.72ms"
          },
          {
            "name": "torus_curvature",
            "value": 5861957,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.59ms"
          }
        ]
      }
    ]
  }
}