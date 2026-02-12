window.BENCHMARK_DATA = {
  "lastUpdate": 1770921876173,
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
          "id": "5d152d8fd23370d975506ed4e667eb7bdca4c5fc",
          "message": "Add macOS ARM64 Embree/SIMD4/SIMD8 benchmark configurations\n\n- Embree supports ARM64 since v3.13, conda-forge has osx-arm64 packages\n- Add macos-arm64-embree, macos-arm64-simd4, macos-arm64-simd8 jobs\n- Update macOS install step to include Embree when needed\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T17:42:58Z",
          "tree_id": "f092267c80309d0261db9099979e779557b5cbb5",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/5d152d8fd23370d975506ed4e667eb7bdca4c5fc"
        },
        "date": 1765647953440,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7161323,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 139.64ms"
          },
          {
            "name": "box",
            "value": 13941751,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 71.73ms"
          },
          {
            "name": "bspline_surface",
            "value": 4469813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 223.72ms"
          },
          {
            "name": "cone",
            "value": 5887097,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 169.86ms"
          },
          {
            "name": "cylinder",
            "value": 15268574,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 65.49ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3752768,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 266.47ms"
          },
          {
            "name": "mr",
            "value": 12705673,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 78.71ms"
          },
          {
            "name": "simple_L021",
            "value": 9386498,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 106.54ms"
          },
          {
            "name": "sphere",
            "value": 5110828,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 195.66ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 5746466,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 174.02ms"
          },
          {
            "name": "torus",
            "value": 7989199,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 125.17ms"
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
        "date": 1765651062162,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 9273771,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 107.83ms"
          },
          {
            "name": "box",
            "value": 25512157,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 39.20ms"
          },
          {
            "name": "bspline_surface",
            "value": 6817562,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 146.68ms"
          },
          {
            "name": "cone",
            "value": 6127977,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 163.19ms"
          },
          {
            "name": "cylinder",
            "value": 14606856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 68.46ms"
          },
          {
            "name": "io1-ca-214",
            "value": 5271426,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 189.70ms"
          },
          {
            "name": "mr",
            "value": 11892445,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 84.09ms"
          },
          {
            "name": "simple_L021",
            "value": 8453085,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 118.30ms"
          },
          {
            "name": "sphere",
            "value": 3133667,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 319.12ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4266412,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 234.39ms"
          },
          {
            "name": "torus",
            "value": 7562981,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 132.22ms"
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
        "date": 1765651250814,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 8440956,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 118.47ms"
          },
          {
            "name": "box",
            "value": 24332084,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 41.10ms"
          },
          {
            "name": "bspline_surface",
            "value": 6382598,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 156.68ms"
          },
          {
            "name": "cone",
            "value": 6576308,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 152.06ms"
          },
          {
            "name": "cylinder",
            "value": 20467477,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 48.86ms"
          },
          {
            "name": "io1-ca-214",
            "value": 5073335,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 197.11ms"
          },
          {
            "name": "mr",
            "value": 11109877,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 90.01ms"
          },
          {
            "name": "simple_L021",
            "value": 7613131,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 131.35ms"
          },
          {
            "name": "sphere",
            "value": 3914568,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 255.46ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 6751146,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 148.12ms"
          },
          {
            "name": "torus",
            "value": 8318568,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 120.21ms"
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
        "date": 1765651513488,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 4968426,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 201.27ms"
          },
          {
            "name": "box",
            "value": 13317707,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 75.09ms"
          },
          {
            "name": "bspline_surface",
            "value": 2696464,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 370.86ms"
          },
          {
            "name": "cone",
            "value": 3598507,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 277.89ms"
          },
          {
            "name": "cylinder",
            "value": 9806325,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 101.97ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2131528,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 469.15ms"
          },
          {
            "name": "mr",
            "value": 7060701,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 141.63ms"
          },
          {
            "name": "simple_L021",
            "value": 6274234,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 159.38ms"
          },
          {
            "name": "sphere",
            "value": 3600269,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 277.76ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4292730,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 232.95ms"
          },
          {
            "name": "torus",
            "value": 5248710,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 190.52ms"
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
        "date": 1765651813642,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5502124,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 181.75ms"
          },
          {
            "name": "box",
            "value": 19286403,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 51.85ms"
          },
          {
            "name": "bspline_surface",
            "value": 5273594,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 189.62ms"
          },
          {
            "name": "cone",
            "value": 4237145,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 236.01ms"
          },
          {
            "name": "cylinder",
            "value": 15972400,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 62.61ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3815163,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 262.11ms"
          },
          {
            "name": "mr",
            "value": 15751256,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 63.49ms"
          },
          {
            "name": "simple_L021",
            "value": 12344917,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 81.01ms"
          },
          {
            "name": "sphere",
            "value": 5883287,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 169.97ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 6657346,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 150.21ms"
          },
          {
            "name": "torus",
            "value": 8405975,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 118.96ms"
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
        "date": 1765653111660,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 6428966,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 155.55ms"
          },
          {
            "name": "box",
            "value": 22239520,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 44.96ms"
          },
          {
            "name": "bspline_surface",
            "value": 4499701,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 222.24ms"
          },
          {
            "name": "cone",
            "value": 4751700,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 210.45ms"
          },
          {
            "name": "cylinder",
            "value": 16166580,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 61.86ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3085953,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 324.05ms"
          },
          {
            "name": "mr",
            "value": 11768860,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 84.97ms"
          },
          {
            "name": "simple_L021",
            "value": 11238986,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 88.98ms"
          },
          {
            "name": "sphere",
            "value": 5265569,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 189.91ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 5575255,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 179.36ms"
          },
          {
            "name": "torus",
            "value": 8185048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 122.17ms"
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
        "date": 1765664547304,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7371478,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 135.66ms"
          },
          {
            "name": "box",
            "value": 22058986,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 45.33ms"
          },
          {
            "name": "bspline_surface",
            "value": 5608022,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 178.32ms"
          },
          {
            "name": "cone",
            "value": 6082022,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.42ms"
          },
          {
            "name": "cylinder",
            "value": 16062451,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 62.26ms"
          },
          {
            "name": "io1-ca-214",
            "value": 5243921,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 190.70ms"
          },
          {
            "name": "mr",
            "value": 13990319,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 71.48ms"
          },
          {
            "name": "simple_L021",
            "value": 8748447,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 114.31ms"
          },
          {
            "name": "sphere",
            "value": 3960976,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 252.46ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4405772,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 226.97ms"
          },
          {
            "name": "torus",
            "value": 5306616,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 188.44ms"
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
        "date": 1765666763911,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 8522098,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 117.34ms"
          },
          {
            "name": "box",
            "value": 28741414,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 34.79ms"
          },
          {
            "name": "bspline_surface",
            "value": 6698642,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 149.28ms"
          },
          {
            "name": "cone",
            "value": 6854808,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 145.88ms"
          },
          {
            "name": "cylinder",
            "value": 17564814,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 56.93ms"
          },
          {
            "name": "io1-ca-214",
            "value": 5099855,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 196.08ms"
          },
          {
            "name": "mr",
            "value": 13469465,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 74.24ms"
          },
          {
            "name": "simple_L021",
            "value": 11840345,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 84.46ms"
          },
          {
            "name": "sphere",
            "value": 5626544,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 177.73ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 5608902,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 178.29ms"
          },
          {
            "name": "torus",
            "value": 8123675,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 123.10ms"
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
        "date": 1765668141626,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 10331219,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 96.79ms"
          },
          {
            "name": "box_raw",
            "value": 22556052,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 44.33ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 10698963,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 93.47ms"
          },
          {
            "name": "cone_raw",
            "value": 14031543,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 71.27ms"
          },
          {
            "name": "cylinder_raw",
            "value": 17670654,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 56.59ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 15206350,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 65.76ms"
          },
          {
            "name": "mr_raw",
            "value": 23920584,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 41.81ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 15895977,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 62.91ms"
          },
          {
            "name": "sphere_raw",
            "value": 15670788,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 63.81ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 17110689,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 58.44ms"
          },
          {
            "name": "torus_raw",
            "value": 19375714,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 51.61ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6425703,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 155.63ms"
          },
          {
            "name": "box_refined",
            "value": 19301293,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 51.81ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 7176896,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 139.34ms"
          },
          {
            "name": "cone_refined",
            "value": 7251316,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 137.91ms"
          },
          {
            "name": "cylinder_refined",
            "value": 19760112,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 50.61ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 5558736,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 179.90ms"
          },
          {
            "name": "mr_refined",
            "value": 16104616,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 62.09ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 11415655,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 87.60ms"
          },
          {
            "name": "sphere_refined",
            "value": 5716278,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 174.94ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 6767502,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 147.77ms"
          },
          {
            "name": "torus_refined",
            "value": 8370232,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 119.47ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 8485795,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 117.84ms"
          },
          {
            "name": "box_normals",
            "value": 22582029,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 44.28ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 5562942,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 179.76ms"
          },
          {
            "name": "cone_normals",
            "value": 5554321,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 180.04ms"
          },
          {
            "name": "cylinder_normals",
            "value": 15701545,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 63.69ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 4739584,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 210.99ms"
          },
          {
            "name": "mr_normals",
            "value": 9736340,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 102.71ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 10238558,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.67ms"
          },
          {
            "name": "sphere_normals",
            "value": 5623570,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 177.82ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 6222233,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 160.71ms"
          },
          {
            "name": "torus_normals",
            "value": 5073103,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 197.12ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 7674303,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 130.30ms"
          },
          {
            "name": "box_curvature",
            "value": 26761581,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 37.37ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 5147449,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 194.27ms"
          },
          {
            "name": "cone_curvature",
            "value": 7173344,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 139.41ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 15564929,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 64.25ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 4855854,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 205.94ms"
          },
          {
            "name": "mr_curvature",
            "value": 7963432,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 125.57ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 7310422,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 136.79ms"
          },
          {
            "name": "sphere_curvature",
            "value": 2918089,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 342.69ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 4316317,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 231.68ms"
          },
          {
            "name": "torus_curvature",
            "value": 5864380,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.52ms"
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
        "date": 1765671098457,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 10632077,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 94.05ms"
          },
          {
            "name": "box_raw",
            "value": 13297519,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 75.20ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 10041572,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 99.59ms"
          },
          {
            "name": "cone_raw",
            "value": 14296130,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 69.95ms"
          },
          {
            "name": "cylinder_raw",
            "value": 16334264,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 61.22ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 6269671,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 159.50ms"
          },
          {
            "name": "mr_raw",
            "value": 22523030,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 44.40ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 14565794,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 68.65ms"
          },
          {
            "name": "sphere_raw",
            "value": 14685582,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 68.09ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 15178422,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 65.88ms"
          },
          {
            "name": "torus_raw",
            "value": 18299601,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 54.65ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 5966160,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 167.61ms"
          },
          {
            "name": "box_refined",
            "value": 13329601,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 75.02ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 4843624,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 206.46ms"
          },
          {
            "name": "cone_refined",
            "value": 5404996,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 185.01ms"
          },
          {
            "name": "cylinder_refined",
            "value": 12380681,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 80.77ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 3408909,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 293.35ms"
          },
          {
            "name": "mr_refined",
            "value": 9224752,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 108.40ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 5384827,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 185.71ms"
          },
          {
            "name": "sphere_refined",
            "value": 2950714,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 338.90ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3499722,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 285.74ms"
          },
          {
            "name": "torus_refined",
            "value": 5951956,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.01ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 5853773,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 170.83ms"
          },
          {
            "name": "box_normals",
            "value": 23901145,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 41.84ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 4351212,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 229.82ms"
          },
          {
            "name": "cone_normals",
            "value": 4661896,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 214.50ms"
          },
          {
            "name": "cylinder_normals",
            "value": 9828783,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 101.74ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 4149154,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 241.01ms"
          },
          {
            "name": "mr_normals",
            "value": 14058963,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 71.13ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 7282154,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 137.32ms"
          },
          {
            "name": "sphere_normals",
            "value": 5229852,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 191.21ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 4776804,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 209.34ms"
          },
          {
            "name": "torus_normals",
            "value": 6016413,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 166.21ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 6716368,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 148.89ms"
          },
          {
            "name": "box_curvature",
            "value": 16880201,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 59.24ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 4379895,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 228.32ms"
          },
          {
            "name": "cone_curvature",
            "value": 5162489,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 193.71ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 8031161,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 124.51ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 3533357,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 283.02ms"
          },
          {
            "name": "mr_curvature",
            "value": 13109768,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 76.28ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 9553927,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 104.67ms"
          },
          {
            "name": "sphere_curvature",
            "value": 5269537,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 189.77ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 6379016,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 156.76ms"
          },
          {
            "name": "torus_curvature",
            "value": 7492769,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 133.46ms"
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
        "date": 1770899335017,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 7853917,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 127.32ms"
          },
          {
            "name": "box_raw",
            "value": 7010853,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 142.64ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 6296991,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 158.81ms"
          },
          {
            "name": "cone_raw",
            "value": 5526694,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 180.94ms"
          },
          {
            "name": "cylinder_raw",
            "value": 5767046,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 173.40ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 4124239,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 242.47ms"
          },
          {
            "name": "mr_raw",
            "value": 8167599,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 122.44ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 7763613,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 128.81ms"
          },
          {
            "name": "sphere_raw",
            "value": 5688185,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 175.80ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 5871163,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 170.32ms"
          },
          {
            "name": "torus_raw",
            "value": 8064191,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 124.01ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 6657479,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 150.21ms"
          },
          {
            "name": "box_refined",
            "value": 20681240,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 48.35ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 3825306,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 261.42ms"
          },
          {
            "name": "cone_refined",
            "value": 4018098,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 248.87ms"
          },
          {
            "name": "cylinder_refined",
            "value": 7932793,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 126.06ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 3535230,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 282.87ms"
          },
          {
            "name": "mr_refined",
            "value": 7817814,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 127.91ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 6338622,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 157.76ms"
          },
          {
            "name": "sphere_refined",
            "value": 3124531,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 320.05ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 4411077,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 226.70ms"
          },
          {
            "name": "torus_refined",
            "value": 5944667,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.22ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 7043196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.98ms"
          },
          {
            "name": "box_normals",
            "value": 20091618,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 49.77ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 5110358,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 195.68ms"
          },
          {
            "name": "cone_normals",
            "value": 4873342,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 205.20ms"
          },
          {
            "name": "cylinder_normals",
            "value": 11477762,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 87.13ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 4515651,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 221.45ms"
          },
          {
            "name": "mr_normals",
            "value": 14118710,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 70.83ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 4553319,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 219.62ms"
          },
          {
            "name": "sphere_normals",
            "value": 3101881,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 322.38ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3163506,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 316.10ms"
          },
          {
            "name": "torus_normals",
            "value": 4105663,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 243.57ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 4326757,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 231.12ms"
          },
          {
            "name": "box_curvature",
            "value": 9790100,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 102.14ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 4251448,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 235.21ms"
          },
          {
            "name": "cone_curvature",
            "value": 4163665,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 240.17ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 6065753,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.86ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 3850463,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 259.71ms"
          },
          {
            "name": "mr_curvature",
            "value": 6893176,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 145.07ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 4875956,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 205.09ms"
          },
          {
            "name": "sphere_curvature",
            "value": 2130688,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 469.33ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 2802274,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 356.85ms"
          },
          {
            "name": "torus_curvature",
            "value": 2954603,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 338.45ms"
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
        "date": 1770921874000,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 6230801,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 160.49ms"
          },
          {
            "name": "box_raw",
            "value": 9627882,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 103.86ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 5212487,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 191.85ms"
          },
          {
            "name": "cone_raw",
            "value": 7972511,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 125.43ms"
          },
          {
            "name": "cylinder_raw",
            "value": 7651812,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 130.69ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 5903501,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 169.39ms"
          },
          {
            "name": "mr_raw",
            "value": 5674467,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 176.23ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 6124374,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 163.28ms"
          },
          {
            "name": "sphere_raw",
            "value": 5112893,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 195.58ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 4979212,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 200.83ms"
          },
          {
            "name": "torus_raw",
            "value": 7030174,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 142.24ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 4834211,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 206.86ms"
          },
          {
            "name": "box_refined",
            "value": 18446441,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 54.21ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 4698784,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 212.82ms"
          },
          {
            "name": "cone_refined",
            "value": 5503729,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 181.69ms"
          },
          {
            "name": "cylinder_refined",
            "value": 8594757,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 116.35ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 2910869,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 343.54ms"
          },
          {
            "name": "mr_refined",
            "value": 5650835,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 176.97ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 4489459,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 222.74ms"
          },
          {
            "name": "sphere_refined",
            "value": 3016864,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 331.47ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 4310048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 232.02ms"
          },
          {
            "name": "torus_refined",
            "value": 4226471,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 236.60ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 5632978,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 177.53ms"
          },
          {
            "name": "box_normals",
            "value": 17588603,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 56.85ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 3076942,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 325.00ms"
          },
          {
            "name": "cone_normals",
            "value": 4623636,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 216.28ms"
          },
          {
            "name": "cylinder_normals",
            "value": 4499296,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 222.26ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 3284687,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 304.44ms"
          },
          {
            "name": "mr_normals",
            "value": 6211296,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 161.00ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 6447162,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 155.11ms"
          },
          {
            "name": "sphere_normals",
            "value": 2435727,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 410.56ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 3348614,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 298.63ms"
          },
          {
            "name": "torus_normals",
            "value": 3951148,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 253.09ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 7892162,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 126.71ms"
          },
          {
            "name": "box_curvature",
            "value": 13180267,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,455, Time: 75.87ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 4756424,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 210.24ms"
          },
          {
            "name": "cone_curvature",
            "value": 5307940,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 188.40ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 9203527,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 108.65ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 3207812,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 311.74ms"
          },
          {
            "name": "mr_curvature",
            "value": 6688382,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 149.51ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 6025839,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 165.95ms"
          },
          {
            "name": "sphere_curvature",
            "value": 3031065,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 329.92ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3885442,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 257.37ms"
          },
          {
            "name": "torus_curvature",
            "value": 5272843,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 189.65ms"
          }
        ]
      }
    ]
  }
}