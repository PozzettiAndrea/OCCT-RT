window.BENCHMARK_DATA = {
  "lastUpdate": 1765651252209,
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
      }
    ]
  }
}