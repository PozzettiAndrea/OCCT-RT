window.BENCHMARK_DATA = {
  "lastUpdate": 1765633999163,
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
          "id": "05b7bcd4e0700565fd6ad54e50b1d156f30a8cf9",
          "message": "Add write permissions for benchmark gh-pages push\n\nGITHUB_TOKEN needs contents:write to push benchmark results to gh-pages.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T13:42:47Z",
          "tree_id": "f3c93447660f3596d9ad5efa36b24f3894860449",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/05b7bcd4e0700565fd6ad54e50b1d156f30a8cf9"
        },
        "date": 1765633475397,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10598834,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 94.35ms"
          },
          {
            "name": "bspline_surface",
            "value": 2960612,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 337.77ms"
          },
          {
            "name": "cone",
            "value": 3467743,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 288.37ms"
          },
          {
            "name": "cylinder",
            "value": 6545574,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 152.78ms"
          },
          {
            "name": "sphere",
            "value": 2353362,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 424.92ms"
          },
          {
            "name": "torus",
            "value": 2722377,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 367.33ms"
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
          "id": "626101f73df76047ad382d3f367df6a04f87bf65",
          "message": "Fix MSVC OpenMP: use signed int for loop variable\n\nMSVC OpenMP requires signed integral types for loop variables.\nChanged size_t to int with static_cast for the loop bound.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T13:51:15Z",
          "tree_id": "becfd0b3a94e3b694cf651731b5ab1ec0f1c6676",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/626101f73df76047ad382d3f367df6a04f87bf65"
        },
        "date": 1765633998329,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10080340,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 99.20ms"
          },
          {
            "name": "bspline_surface",
            "value": 2870462,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 348.38ms"
          },
          {
            "name": "cone",
            "value": 3363323,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 297.33ms"
          },
          {
            "name": "cylinder",
            "value": 6569568,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 152.22ms"
          },
          {
            "name": "sphere",
            "value": 2318695,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 431.28ms"
          },
          {
            "name": "torus",
            "value": 2733645,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 365.81ms"
          }
        ]
      }
    ]
  }
}