window.BENCHMARK_DATA = {
  "lastUpdate": 1765648312083,
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
      }
    ]
  }
}