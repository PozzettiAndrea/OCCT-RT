window.BENCHMARK_DATA = {
  "lastUpdate": 1770922082884,
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
        "date": 1765645764388,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 2754523,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 363.04ms"
          },
          {
            "name": "box",
            "value": 9328045,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 107.20ms"
          },
          {
            "name": "bspline_surface",
            "value": 2412211,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 414.56ms"
          },
          {
            "name": "cone",
            "value": 3691432,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 270.90ms"
          },
          {
            "name": "cylinder",
            "value": 9311750,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 107.39ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1798876,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 555.90ms"
          },
          {
            "name": "mr",
            "value": 6148315,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 162.65ms"
          },
          {
            "name": "simple_L021",
            "value": 5837483,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 171.31ms"
          },
          {
            "name": "sphere",
            "value": 3102536,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 322.32ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3016925,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 331.46ms"
          },
          {
            "name": "torus",
            "value": 5243973,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 190.70ms"
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
        "date": 1765648306819,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 2907457,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 343.94ms"
          },
          {
            "name": "box",
            "value": 10760232,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.93ms"
          },
          {
            "name": "bspline_surface",
            "value": 2675897,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 373.71ms"
          },
          {
            "name": "cone",
            "value": 4109331,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 243.35ms"
          },
          {
            "name": "cylinder",
            "value": 11428075,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 87.50ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2112784,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 473.31ms"
          },
          {
            "name": "mr",
            "value": 7619024,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 131.25ms"
          },
          {
            "name": "simple_L021",
            "value": 6610530,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 151.27ms"
          },
          {
            "name": "sphere",
            "value": 3400280,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 294.09ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2741433,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 364.77ms"
          },
          {
            "name": "torus",
            "value": 4846828,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 206.32ms"
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
        "date": 1765651359139,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5804717,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 172.27ms"
          },
          {
            "name": "box",
            "value": 12482337,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.11ms"
          },
          {
            "name": "bspline_surface",
            "value": 3478955,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 287.44ms"
          },
          {
            "name": "cone",
            "value": 5157609,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 193.89ms"
          },
          {
            "name": "cylinder",
            "value": 12752890,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.41ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2728744,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 366.47ms"
          },
          {
            "name": "mr",
            "value": 9094985,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 109.95ms"
          },
          {
            "name": "simple_L021",
            "value": 7990648,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.15ms"
          },
          {
            "name": "sphere",
            "value": 4293672,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 232.90ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3489678,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 286.56ms"
          },
          {
            "name": "torus",
            "value": 5900022,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 169.49ms"
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
        "date": 1765651558854,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5886900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 169.87ms"
          },
          {
            "name": "box",
            "value": 13214159,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 75.68ms"
          },
          {
            "name": "bspline_surface",
            "value": 3470998,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 288.10ms"
          },
          {
            "name": "cone",
            "value": 5151676,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 194.11ms"
          },
          {
            "name": "cylinder",
            "value": 13299871,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 75.19ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2710936,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 368.88ms"
          },
          {
            "name": "mr",
            "value": 7527043,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 132.85ms"
          },
          {
            "name": "simple_L021",
            "value": 7426998,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 134.64ms"
          },
          {
            "name": "sphere",
            "value": 3645177,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 274.34ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3427212,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 291.78ms"
          },
          {
            "name": "torus",
            "value": 4822701,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 207.35ms"
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
        "date": 1765651804032,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5724580,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 174.69ms"
          },
          {
            "name": "box",
            "value": 12463839,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.23ms"
          },
          {
            "name": "bspline_surface",
            "value": 3479402,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 287.41ms"
          },
          {
            "name": "cone",
            "value": 5223591,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 191.44ms"
          },
          {
            "name": "cylinder",
            "value": 13668634,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 73.16ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2688611,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 371.94ms"
          },
          {
            "name": "mr",
            "value": 8882138,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 112.59ms"
          },
          {
            "name": "simple_L021",
            "value": 8027937,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 124.57ms"
          },
          {
            "name": "sphere",
            "value": 4310226,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 232.01ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3274105,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 305.43ms"
          },
          {
            "name": "torus",
            "value": 5888893,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 169.81ms"
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
        "date": 1765652169838,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 2810539,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 355.80ms"
          },
          {
            "name": "box",
            "value": 10088863,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 99.12ms"
          },
          {
            "name": "bspline_surface",
            "value": 2768148,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 361.25ms"
          },
          {
            "name": "cone",
            "value": 3821721,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 261.66ms"
          },
          {
            "name": "cylinder",
            "value": 11184894,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.41ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2117419,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 472.27ms"
          },
          {
            "name": "mr",
            "value": 7608017,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 131.44ms"
          },
          {
            "name": "simple_L021",
            "value": 6757455,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 147.98ms"
          },
          {
            "name": "sphere",
            "value": 3336148,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 299.75ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3498551,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 285.83ms"
          },
          {
            "name": "torus",
            "value": 6097405,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 164.00ms"
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
        "date": 1765653482867,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 3503228,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 285.45ms"
          },
          {
            "name": "box",
            "value": 12195464,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 82.00ms"
          },
          {
            "name": "bspline_surface",
            "value": 3288901,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 304.05ms"
          },
          {
            "name": "cone",
            "value": 4692934,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 213.09ms"
          },
          {
            "name": "cylinder",
            "value": 13438675,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 74.41ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2565913,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 389.72ms"
          },
          {
            "name": "mr",
            "value": 8603142,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 116.24ms"
          },
          {
            "name": "simple_L021",
            "value": 7678009,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 130.24ms"
          },
          {
            "name": "sphere",
            "value": 4046060,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 247.15ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3242066,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 308.45ms"
          },
          {
            "name": "torus",
            "value": 2777522,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 360.03ms"
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
        "date": 1765664906624,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 4612010,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 216.83ms"
          },
          {
            "name": "box",
            "value": 10772774,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.83ms"
          },
          {
            "name": "bspline_surface",
            "value": 2746299,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 364.13ms"
          },
          {
            "name": "cone",
            "value": 4087987,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 244.62ms"
          },
          {
            "name": "cylinder",
            "value": 10356888,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 96.55ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2088638,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 478.78ms"
          },
          {
            "name": "mr",
            "value": 7314641,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 136.71ms"
          },
          {
            "name": "simple_L021",
            "value": 6524924,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 153.26ms"
          },
          {
            "name": "sphere",
            "value": 3386422,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 295.30ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2703312,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 369.92ms"
          },
          {
            "name": "torus",
            "value": 4677413,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 213.79ms"
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
        "date": 1765667092468,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5968090,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 167.56ms"
          },
          {
            "name": "box",
            "value": 12901694,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 77.51ms"
          },
          {
            "name": "bspline_surface",
            "value": 3576287,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 279.62ms"
          },
          {
            "name": "cone",
            "value": 5297014,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 188.79ms"
          },
          {
            "name": "cylinder",
            "value": 13807654,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 72.42ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2761987,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 362.06ms"
          },
          {
            "name": "mr",
            "value": 9337644,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 107.09ms"
          },
          {
            "name": "simple_L021",
            "value": 7985785,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.22ms"
          },
          {
            "name": "sphere",
            "value": 4339274,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 230.45ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3558900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 280.99ms"
          },
          {
            "name": "torus",
            "value": 6142910,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 162.79ms"
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
        "date": 1765668519701,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 8683103,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 115.17ms"
          },
          {
            "name": "box_raw",
            "value": 12472078,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.18ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8129288,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 123.01ms"
          },
          {
            "name": "cone_raw",
            "value": 11251809,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 88.87ms"
          },
          {
            "name": "cylinder_raw",
            "value": 13375403,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 74.76ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9445341,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 105.87ms"
          },
          {
            "name": "mr_raw",
            "value": 16234162,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 61.60ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 10338482,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 96.73ms"
          },
          {
            "name": "sphere_raw",
            "value": 10760232,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 92.93ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10635390,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 94.03ms"
          },
          {
            "name": "torus_raw",
            "value": 12341108,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 81.03ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6124329,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 163.28ms"
          },
          {
            "name": "box_refined",
            "value": 12592333,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.41ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3846657,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 259.97ms"
          },
          {
            "name": "cone_refined",
            "value": 5321125,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 187.93ms"
          },
          {
            "name": "cylinder_refined",
            "value": 13809904,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 72.41ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 2535595,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 394.38ms"
          },
          {
            "name": "mr_refined",
            "value": 9667945,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 103.43ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 7966382,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.53ms"
          },
          {
            "name": "sphere_refined",
            "value": 4335412,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 230.66ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3563692,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 280.61ms"
          },
          {
            "name": "torus_refined",
            "value": 6225596,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 160.63ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 2980055,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 335.56ms"
          },
          {
            "name": "box_normals",
            "value": 11841691,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 84.45ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 3573301,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 279.85ms"
          },
          {
            "name": "cone_normals",
            "value": 4904993,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 203.87ms"
          },
          {
            "name": "cylinder_normals",
            "value": 13290008,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 75.24ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2795324,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 357.74ms"
          },
          {
            "name": "mr_normals",
            "value": 8794626,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 113.71ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 7499754,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 133.34ms"
          },
          {
            "name": "sphere_normals",
            "value": 4209410,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 237.56ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3399335,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 294.18ms"
          },
          {
            "name": "torus_normals",
            "value": 5729753,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 174.53ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 3570258,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 280.09ms"
          },
          {
            "name": "box_curvature",
            "value": 12388304,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.72ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3154802,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 316.98ms"
          },
          {
            "name": "cone_curvature",
            "value": 4844069,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 206.44ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 13004916,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 76.89ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2233170,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 447.79ms"
          },
          {
            "name": "mr_curvature",
            "value": 6199478,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 161.30ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 7812927,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 127.99ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4130015,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 242.13ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 2847197,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 351.22ms"
          },
          {
            "name": "torus_curvature",
            "value": 5309950,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 188.33ms"
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
          "id": "b83b38c19805e9c8909d2f29e0bc831a40faa02f",
          "message": "night",
          "timestamp": "2025-12-14T00:05:12Z",
          "tree_id": "f1a2c0e73f7f42e0f6df24745c6fd58665efa72c",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/b83b38c19805e9c8909d2f29e0bc831a40faa02f"
        },
        "date": 1765671578619,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 6548240,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 152.71ms"
          },
          {
            "name": "box_raw",
            "value": 12849870,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 77.82ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8477924,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 117.95ms"
          },
          {
            "name": "cone_raw",
            "value": 11177880,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.46ms"
          },
          {
            "name": "cylinder_raw",
            "value": 12745218,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.46ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9075753,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 110.18ms"
          },
          {
            "name": "mr_raw",
            "value": 15754854,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 63.47ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 9711161,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 102.97ms"
          },
          {
            "name": "sphere_raw",
            "value": 10106410,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 98.95ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10695141,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 93.50ms"
          },
          {
            "name": "torus_raw",
            "value": 12018422,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 83.21ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6087383,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 164.27ms"
          },
          {
            "name": "box_refined",
            "value": 13021037,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 76.80ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3857782,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 259.22ms"
          },
          {
            "name": "cone_refined",
            "value": 5327719,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 187.70ms"
          },
          {
            "name": "cylinder_refined",
            "value": 11823364,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 84.58ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 3038015,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 329.16ms"
          },
          {
            "name": "mr_refined",
            "value": 7898582,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 126.60ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 6448347,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 155.08ms"
          },
          {
            "name": "sphere_refined",
            "value": 3399537,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 294.16ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 2820268,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 354.58ms"
          },
          {
            "name": "torus_refined",
            "value": 4683742,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 213.50ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 4599285,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 217.43ms"
          },
          {
            "name": "box_normals",
            "value": 9743303,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 102.63ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 2873185,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 348.05ms"
          },
          {
            "name": "cone_normals",
            "value": 4030550,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 248.11ms"
          },
          {
            "name": "cylinder_normals",
            "value": 10588453,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 94.44ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2261188,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 442.25ms"
          },
          {
            "name": "mr_normals",
            "value": 7957102,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 125.67ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 6206812,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 161.11ms"
          },
          {
            "name": "sphere_normals",
            "value": 4275933,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 233.87ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3535363,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 282.86ms"
          },
          {
            "name": "torus_normals",
            "value": 5943770,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.24ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 4525882,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 220.95ms"
          },
          {
            "name": "box_curvature",
            "value": 12102538,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 82.63ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3471951,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 288.02ms"
          },
          {
            "name": "cone_curvature",
            "value": 4967685,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 201.30ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 12733760,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.53ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2578336,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 387.85ms"
          },
          {
            "name": "mr_curvature",
            "value": 8961858,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 111.58ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 7912300,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 126.39ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4230389,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 236.38ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3301872,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 302.86ms"
          },
          {
            "name": "torus_curvature",
            "value": 5836897,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 171.32ms"
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
          "id": "433f740839ccd21fcefade82497d7a66cd8c905c",
          "message": "night",
          "timestamp": "2025-12-14T00:05:12Z",
          "tree_id": "f1a2c0e73f7f42e0f6df24745c6fd58665efa72c",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/433f740839ccd21fcefade82497d7a66cd8c905c"
        },
        "date": 1770899587475,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 9041845,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 110.60ms"
          },
          {
            "name": "box_raw",
            "value": 12805589,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 78.09ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8344619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 119.84ms"
          },
          {
            "name": "cone_raw",
            "value": 10950432,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 91.32ms"
          },
          {
            "name": "cylinder_raw",
            "value": 13114049,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 76.25ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9138529,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 109.43ms"
          },
          {
            "name": "mr_raw",
            "value": 15503780,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 64.50ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 9984045,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 100.16ms"
          },
          {
            "name": "sphere_raw",
            "value": 10365670,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 96.47ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10337488,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 96.74ms"
          },
          {
            "name": "torus_raw",
            "value": 11874905,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 84.21ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6100362,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 163.92ms"
          },
          {
            "name": "box_refined",
            "value": 12494112,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.04ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3675883,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 272.04ms"
          },
          {
            "name": "cone_refined",
            "value": 5099598,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 196.09ms"
          },
          {
            "name": "cylinder_refined",
            "value": 12933766,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 77.32ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 2925553,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 341.82ms"
          },
          {
            "name": "mr_refined",
            "value": 9221647,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 108.44ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 7944983,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 125.87ms"
          },
          {
            "name": "sphere_refined",
            "value": 4198872,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 238.16ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3467914,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 288.36ms"
          },
          {
            "name": "torus_refined",
            "value": 5715553,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 174.96ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 4557947,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 219.40ms"
          },
          {
            "name": "box_normals",
            "value": 12226015,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 81.79ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 3660046,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 273.22ms"
          },
          {
            "name": "cone_normals",
            "value": 5075720,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 197.02ms"
          },
          {
            "name": "cylinder_normals",
            "value": 12605937,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 79.33ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2952621,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 338.68ms"
          },
          {
            "name": "mr_normals",
            "value": 9157744,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 109.20ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 7726011,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 129.43ms"
          },
          {
            "name": "sphere_normals",
            "value": 3883378,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 257.51ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3418421,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 292.53ms"
          },
          {
            "name": "torus_normals",
            "value": 5654602,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 176.85ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 3925037,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 254.77ms"
          },
          {
            "name": "box_curvature",
            "value": 10173727,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 98.29ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3304982,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 302.57ms"
          },
          {
            "name": "cone_curvature",
            "value": 4041766,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 247.42ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 10717206,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 93.31ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2189545,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 456.72ms"
          },
          {
            "name": "mr_curvature",
            "value": 8880900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 112.60ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 7802363,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 128.17ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4212385,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 237.40ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3431728,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 291.40ms"
          },
          {
            "name": "torus_curvature",
            "value": 5585272,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 179.04ms"
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
          "id": "ee018d02388c9a98f2f89c6417a11cf15c9ed726",
          "message": "night",
          "timestamp": "2025-12-14T01:05:12+01:00",
          "tree_id": "f1a2c0e73f7f42e0f6df24745c6fd58665efa72c",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/ee018d02388c9a98f2f89c6417a11cf15c9ed726"
        },
        "date": 1770922078842,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 9140952,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 109.40ms"
          },
          {
            "name": "box_raw",
            "value": 13004544,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 76.90ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 8496343,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 117.70ms"
          },
          {
            "name": "cone_raw",
            "value": 10951595,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 91.31ms"
          },
          {
            "name": "cylinder_raw",
            "value": 13139931,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 76.10ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 9069036,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 110.27ms"
          },
          {
            "name": "mr_raw",
            "value": 15351103,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 65.14ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 10029567,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 99.71ms"
          },
          {
            "name": "sphere_raw",
            "value": 9717606,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 102.91ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 10443363,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 95.75ms"
          },
          {
            "name": "torus_raw",
            "value": 11993672,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 83.38ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6119708,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 163.41ms"
          },
          {
            "name": "box_refined",
            "value": 12901311,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 77.51ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3824481,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 261.47ms"
          },
          {
            "name": "cone_refined",
            "value": 5278006,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 189.47ms"
          },
          {
            "name": "cylinder_refined",
            "value": 12815222,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.03ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 3088679,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 323.76ms"
          },
          {
            "name": "mr_refined",
            "value": 9703236,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 103.06ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 8075014,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 123.84ms"
          },
          {
            "name": "sphere_refined",
            "value": 4431204,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 225.67ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3622495,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 276.05ms"
          },
          {
            "name": "torus_refined",
            "value": 5758504,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 173.66ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 4840222,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 206.60ms"
          },
          {
            "name": "box_normals",
            "value": 10771939,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.83ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 2846004,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 351.37ms"
          },
          {
            "name": "cone_normals",
            "value": 3724353,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 268.50ms"
          },
          {
            "name": "cylinder_normals",
            "value": 10999140,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 90.92ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2237783,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 446.87ms"
          },
          {
            "name": "mr_normals",
            "value": 9612011,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 104.04ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 8158224,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 122.58ms"
          },
          {
            "name": "sphere_normals",
            "value": 4465863,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 223.92ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3632769,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 275.27ms"
          },
          {
            "name": "torus_normals",
            "value": 6144152,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 162.76ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 5848986,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 170.97ms"
          },
          {
            "name": "box_curvature",
            "value": 12611788,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.29ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3541320,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 282.38ms"
          },
          {
            "name": "cone_curvature",
            "value": 5143940,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 194.40ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 12845083,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 77.85ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2749972,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 363.64ms"
          },
          {
            "name": "mr_curvature",
            "value": 9148101,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 109.31ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 8071566,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 123.89ms"
          },
          {
            "name": "sphere_curvature",
            "value": 4281916,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 233.54ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3482480,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 287.15ms"
          },
          {
            "name": "torus_curvature",
            "value": 5943649,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.25ms"
          }
        ]
      }
    ]
  }
}