window.BENCHMARK_DATA = {
  "lastUpdate": 1765640438482,
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
          "id": "fddf48f2c39b8d1c7fe500f7b3625783af086612",
          "message": "Expand CI matrix to all 4 backends with shaded renders\n\n- Add OCCT, Embree, SIMD4, SIMD8 backend configurations for Linux\n- Update benchmark parser to accept --backend flag\n- Create render_shaded.py for Lambertian-shaded geometry renders\n- Upload rendered images to gh-pages for dashboard display\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T15:34:35Z",
          "tree_id": "19ca2878454bc724ac218699054106fa4a3985c2",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/fddf48f2c39b8d1c7fe500f7b3625783af086612"
        },
        "date": 1765640188215,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19159290,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.19ms"
          },
          {
            "name": "bspline_surface",
            "value": 3797242,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 263.35ms"
          },
          {
            "name": "cone",
            "value": 6039596,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 165.57ms"
          },
          {
            "name": "cylinder",
            "value": 19334880,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.72ms"
          },
          {
            "name": "sphere",
            "value": 5024722,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 199.02ms"
          },
          {
            "name": "torus",
            "value": 7129566,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.26ms"
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
          "id": "786108fec2805b0e864a7e7dbaeae36920f2fb25",
          "message": "Fix render script: use absolute paths for subprocess",
          "timestamp": "2025-12-13T15:38:45Z",
          "tree_id": "29610083dbc0bc4630ce905dd6c1ee6b7216bb4a",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/786108fec2805b0e864a7e7dbaeae36920f2fb25"
        },
        "date": 1765640438177,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19721143,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.71ms"
          },
          {
            "name": "bspline_surface",
            "value": 3895537,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 256.70ms"
          },
          {
            "name": "cone",
            "value": 6055798,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 165.13ms"
          },
          {
            "name": "cylinder",
            "value": 19615920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 50.98ms"
          },
          {
            "name": "sphere",
            "value": 5028385,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 198.87ms"
          },
          {
            "name": "torus",
            "value": 7290064,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 137.17ms"
          }
        ]
      }
    ]
  }
}