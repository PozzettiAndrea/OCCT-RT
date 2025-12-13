window.BENCHMARK_DATA = {
  "lastUpdate": 1765668532516,
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
        "date": 1765645740119,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5651240,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 176.95ms"
          },
          {
            "name": "box",
            "value": 11540987,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 86.65ms"
          },
          {
            "name": "bspline_surface",
            "value": 3132691,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 319.21ms"
          },
          {
            "name": "cone",
            "value": 4745846,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 210.71ms"
          },
          {
            "name": "cylinder",
            "value": 12280486,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.43ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2596235,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 385.17ms"
          },
          {
            "name": "mr",
            "value": 7528681,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 132.83ms"
          },
          {
            "name": "simple_L021",
            "value": 6935712,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 144.18ms"
          },
          {
            "name": "sphere",
            "value": 4006123,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 249.62ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3265908,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 306.19ms"
          },
          {
            "name": "torus",
            "value": 5759168,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 173.64ms"
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
        "date": 1765648289059,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5817674,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 171.89ms"
          },
          {
            "name": "box",
            "value": 12008377,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 83.28ms"
          },
          {
            "name": "bspline_surface",
            "value": 3467790,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 288.37ms"
          },
          {
            "name": "cone",
            "value": 5102840,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 195.97ms"
          },
          {
            "name": "cylinder",
            "value": 12316818,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.19ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2658012,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 376.22ms"
          },
          {
            "name": "mr",
            "value": 8491105,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 117.77ms"
          },
          {
            "name": "simple_L021",
            "value": 7852468,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 127.35ms"
          },
          {
            "name": "sphere",
            "value": 4251299,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 235.22ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3389997,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 294.99ms"
          },
          {
            "name": "torus",
            "value": 5864061,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.53ms"
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
        "date": 1765651473589,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 3355260,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 298.04ms"
          },
          {
            "name": "box",
            "value": 12481590,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.12ms"
          },
          {
            "name": "bspline_surface",
            "value": 3550660,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 281.64ms"
          },
          {
            "name": "cone",
            "value": 5206216,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 192.08ms"
          },
          {
            "name": "cylinder",
            "value": 12434764,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.42ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2708671,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 369.18ms"
          },
          {
            "name": "mr",
            "value": 8771553,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.00ms"
          },
          {
            "name": "simple_L021",
            "value": 7929900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 126.10ms"
          },
          {
            "name": "sphere",
            "value": 4359008,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 229.41ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3498616,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 285.83ms"
          },
          {
            "name": "torus",
            "value": 5976943,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 167.31ms"
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
        "date": 1765651520902,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5624658,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 177.79ms"
          },
          {
            "name": "box",
            "value": 12378199,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.79ms"
          },
          {
            "name": "bspline_surface",
            "value": 3466323,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 288.49ms"
          },
          {
            "name": "cone",
            "value": 5100623,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 196.05ms"
          },
          {
            "name": "cylinder",
            "value": 12482836,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.11ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2583279,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 387.10ms"
          },
          {
            "name": "mr",
            "value": 9043333,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 110.58ms"
          },
          {
            "name": "simple_L021",
            "value": 8197702,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 121.99ms"
          },
          {
            "name": "sphere",
            "value": 4399025,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 227.32ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3321444,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 301.07ms"
          },
          {
            "name": "torus",
            "value": 6043947,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 165.45ms"
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
        "date": 1765651777477,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5861167,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 170.61ms"
          },
          {
            "name": "box",
            "value": 12250128,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 81.63ms"
          },
          {
            "name": "bspline_surface",
            "value": 3539968,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 282.49ms"
          },
          {
            "name": "cone",
            "value": 5161447,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 193.74ms"
          },
          {
            "name": "cylinder",
            "value": 12473260,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.17ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2742856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.58ms"
          },
          {
            "name": "mr",
            "value": 8758608,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.17ms"
          },
          {
            "name": "simple_L021",
            "value": 7972790,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.43ms"
          },
          {
            "name": "sphere",
            "value": 4358856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 229.42ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3449813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 289.87ms"
          },
          {
            "name": "torus",
            "value": 5926973,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.72ms"
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
        "date": 1765652221586,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5794273,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 172.58ms"
          },
          {
            "name": "box",
            "value": 12487778,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.08ms"
          },
          {
            "name": "bspline_surface",
            "value": 3574653,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 279.75ms"
          },
          {
            "name": "cone",
            "value": 5270114,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 189.75ms"
          },
          {
            "name": "cylinder",
            "value": 12572765,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 79.54ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2741720,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.73ms"
          },
          {
            "name": "mr",
            "value": 9059194,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 110.39ms"
          },
          {
            "name": "simple_L021",
            "value": 8031916,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 124.50ms"
          },
          {
            "name": "sphere",
            "value": 4065712,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 245.96ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3411306,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 293.14ms"
          },
          {
            "name": "torus",
            "value": 5986766,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 167.04ms"
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
        "date": 1765653445149,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5866961,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 170.45ms"
          },
          {
            "name": "box",
            "value": 13149054,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 76.05ms"
          },
          {
            "name": "bspline_surface",
            "value": 3557902,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 281.06ms"
          },
          {
            "name": "cone",
            "value": 5241293,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 190.79ms"
          },
          {
            "name": "cylinder",
            "value": 13145493,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 76.07ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2721315,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 367.47ms"
          },
          {
            "name": "mr",
            "value": 9096400,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 109.93ms"
          },
          {
            "name": "simple_L021",
            "value": 8249124,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 121.23ms"
          },
          {
            "name": "sphere",
            "value": 4376186,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 228.51ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2899196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 344.92ms"
          },
          {
            "name": "torus",
            "value": 6049896,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 165.29ms"
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
        "date": 1765664925687,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 2357644,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 424.15ms"
          },
          {
            "name": "box",
            "value": 10411504,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 96.05ms"
          },
          {
            "name": "bspline_surface",
            "value": 2768350,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 361.23ms"
          },
          {
            "name": "cone",
            "value": 4111786,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 243.20ms"
          },
          {
            "name": "cylinder",
            "value": 10350981,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 96.61ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2133178,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 468.78ms"
          },
          {
            "name": "mr",
            "value": 7227919,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 138.35ms"
          },
          {
            "name": "simple_L021",
            "value": 6689881,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 149.48ms"
          },
          {
            "name": "sphere",
            "value": 3455054,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 289.43ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3523485,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 283.81ms"
          },
          {
            "name": "torus",
            "value": 6095866,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 164.05ms"
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
        "date": 1765667063198,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5886047,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 169.89ms"
          },
          {
            "name": "box",
            "value": 12397673,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.66ms"
          },
          {
            "name": "bspline_surface",
            "value": 3533548,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 283.00ms"
          },
          {
            "name": "cone",
            "value": 4929404,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 202.86ms"
          },
          {
            "name": "cylinder",
            "value": 12307813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.25ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2684285,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 372.54ms"
          },
          {
            "name": "mr",
            "value": 8726019,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 114.60ms"
          },
          {
            "name": "simple_L021",
            "value": 7744512,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 129.12ms"
          },
          {
            "name": "sphere",
            "value": 4274575,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 233.94ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3396996,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 294.38ms"
          },
          {
            "name": "torus",
            "value": 5767668,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 173.38ms"
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
        "date": 1765668523076,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 9307451,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 107.44ms"
          },
          {
            "name": "box_raw",
            "value": 13064876,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 76.54ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8801329,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 113.62ms"
          },
          {
            "name": "cone_raw",
            "value": 11362048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 88.01ms"
          },
          {
            "name": "cylinder_raw",
            "value": 12971077,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 77.09ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9273582,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 107.83ms"
          },
          {
            "name": "mr_raw",
            "value": 15686373,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 63.75ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 10166931,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.36ms"
          },
          {
            "name": "sphere_raw",
            "value": 10893934,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 91.79ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10616669,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 94.19ms"
          },
          {
            "name": "torus_raw",
            "value": 12625800,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 79.20ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 4611778,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 216.84ms"
          },
          {
            "name": "box_refined",
            "value": 9601462,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 104.15ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 2661567,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 375.72ms"
          },
          {
            "name": "cone_refined",
            "value": 2691984,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 371.47ms"
          },
          {
            "name": "cylinder_refined",
            "value": 7214851,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 138.60ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 1833157,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 545.51ms"
          },
          {
            "name": "mr_refined",
            "value": 6214716,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 160.91ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 5469275,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 182.84ms"
          },
          {
            "name": "sphere_refined",
            "value": 2562623,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 390.23ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 2101859,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 475.77ms"
          },
          {
            "name": "torus_refined",
            "value": 3534619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 282.92ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 6005252,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 166.52ms"
          },
          {
            "name": "box_normals",
            "value": 12392004,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.70ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 3873591,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 258.16ms"
          },
          {
            "name": "cone_normals",
            "value": 5404076,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 185.05ms"
          },
          {
            "name": "cylinder_normals",
            "value": 11197506,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.31ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2995834,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 333.80ms"
          },
          {
            "name": "mr_normals",
            "value": 9492331,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 105.35ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 8077740,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 123.80ms"
          },
          {
            "name": "sphere_normals",
            "value": 4346420,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 230.07ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3461813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 288.87ms"
          },
          {
            "name": "torus_normals",
            "value": 5872622,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.28ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 3314615,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 301.69ms"
          },
          {
            "name": "box_curvature",
            "value": 12180194,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 82.10ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 2764786,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 361.69ms"
          },
          {
            "name": "cone_curvature",
            "value": 5041044,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 198.37ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 10768969,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 92.86ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2606325,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 383.68ms"
          },
          {
            "name": "mr_curvature",
            "value": 6411625,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 155.97ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 7711932,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 129.67ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4203358,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 237.91ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3347038,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 298.77ms"
          },
          {
            "name": "torus_curvature",
            "value": 5634539,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 177.48ms"
          }
        ]
      }
    ]
  }
}