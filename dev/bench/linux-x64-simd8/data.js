window.BENCHMARK_DATA = {
  "lastUpdate": 1765640835239,
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
        "date": 1765640190704,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19344605,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.69ms"
          },
          {
            "name": "bspline_surface",
            "value": 3986049,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 250.88ms"
          },
          {
            "name": "cone",
            "value": 6206053,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 161.13ms"
          },
          {
            "name": "cylinder",
            "value": 19766752,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 50.59ms"
          },
          {
            "name": "sphere",
            "value": 5076065,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 197.00ms"
          },
          {
            "name": "torus",
            "value": 7391475,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 135.29ms"
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
        "date": 1765640434732,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19573685,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.09ms"
          },
          {
            "name": "bspline_surface",
            "value": 3683241,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 271.50ms"
          },
          {
            "name": "cone",
            "value": 6147302,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 162.67ms"
          },
          {
            "name": "cylinder",
            "value": 19441636,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.44ms"
          },
          {
            "name": "sphere",
            "value": 5099127,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 196.11ms"
          },
          {
            "name": "torus",
            "value": 7244539,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 138.04ms"
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
          "id": "41f1e7d379f7c277b8a81d0a4b26bd4c3ee6bd32",
          "message": "Fix render: add --allow-disconnected flag and debug output",
          "timestamp": "2025-12-13T15:45:25Z",
          "tree_id": "96cd32a57c2c5d4e1595cdea279d70cfa57071d8",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/41f1e7d379f7c277b8a81d0a4b26bd4c3ee6bd32"
        },
        "date": 1765640834548,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19885855,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.29ms"
          },
          {
            "name": "bspline_surface",
            "value": 4061095,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 246.24ms"
          },
          {
            "name": "cone",
            "value": 6120513,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 163.38ms"
          },
          {
            "name": "cylinder",
            "value": 19126310,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 52.28ms"
          },
          {
            "name": "sphere",
            "value": 5161130,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 193.76ms"
          },
          {
            "name": "torus",
            "value": 7386997,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 135.37ms"
          }
        ]
      }
    ]
  }
}