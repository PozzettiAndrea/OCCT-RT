window.BENCHMARK_DATA = {
  "lastUpdate": 1765668170329,
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
          "id": "1fddec468bf210147ed953363a6c5537c2714778",
          "message": "Add conda DLL path for Windows benchmark execution\n\nOCCT and Embree DLLs are in $CONDA_PREFIX/Library/bin on Windows.\nAdd to PATH before running raytracer benchmarks.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T13:59:32Z",
          "tree_id": "79b30be484c12deac23d695cb26256a1b2eea637",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/1fddec468bf210147ed953363a6c5537c2714778"
        },
        "date": 1765634487619,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10452379,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 95.67ms"
          },
          {
            "name": "bspline_surface",
            "value": 2885220,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 346.59ms"
          },
          {
            "name": "cone",
            "value": 3398540,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 294.24ms"
          },
          {
            "name": "cylinder",
            "value": 6628926,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 150.85ms"
          },
          {
            "name": "sphere",
            "value": 2396765,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 417.23ms"
          },
          {
            "name": "torus",
            "value": 2752599,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 363.29ms"
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
          "id": "a09e68967cb81d09e8fce6ad023de3dca9da8f90",
          "message": "Make Windows benchmarks optional due to DLL loading issues\n\nWindows conda environment DLL paths are not reliably inherited by\nsubprocess calls, causing benchmark parsing to fail. The build itself\nsucceeds, so we allow the benchmark step to fail without blocking CI.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T14:09:21Z",
          "tree_id": "bb2e2e307ff6e0de8e86be43b67fdb40400ea7c9",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/a09e68967cb81d09e8fce6ad023de3dca9da8f90"
        },
        "date": 1765635078477,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10082373,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 99.18ms"
          },
          {
            "name": "bspline_surface",
            "value": 2938489,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 340.31ms"
          },
          {
            "name": "cone",
            "value": 3447681,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 290.05ms"
          },
          {
            "name": "cylinder",
            "value": 6564393,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 152.34ms"
          },
          {
            "name": "sphere",
            "value": 2406768,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 415.49ms"
          },
          {
            "name": "torus",
            "value": 2723022,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 367.24ms"
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
          "id": "a756eeec3540552b1a5d4e7564f32ec2aa326a64",
          "message": "Skip benchmark storage when no results collected\n\nAdd validity check step to verify benchmark_results.json has actual\nentries before attempting to store them. This prevents CI failure on\nWindows where DLL loading issues cause empty benchmark results.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T14:18:32Z",
          "tree_id": "28892ca8281c462f42da1e8f5fc3dd2e30281aa3",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/a756eeec3540552b1a5d4e7564f32ec2aa326a64"
        },
        "date": 1765635623736,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10513920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 95.11ms"
          },
          {
            "name": "bspline_surface",
            "value": 2959175,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 337.93ms"
          },
          {
            "name": "cone",
            "value": 3456512,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.31ms"
          },
          {
            "name": "cylinder",
            "value": 6608861,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 151.31ms"
          },
          {
            "name": "sphere",
            "value": 2401577,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 416.39ms"
          },
          {
            "name": "torus",
            "value": 2747215,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 364.00ms"
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
          "id": "ddfe28e9e4ee7c9ea248a5919f99247d174c4636",
          "message": "Add CI badge and benchmark results link to README\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T14:33:46Z",
          "tree_id": "8f69ecc7d8edbdeb6d157742dc75890b5ac17376",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/ddfe28e9e4ee7c9ea248a5919f99247d174c4636"
        },
        "date": 1765636534199,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10345113,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 96.66ms"
          },
          {
            "name": "bspline_surface",
            "value": 2834234,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 352.83ms"
          },
          {
            "name": "cone",
            "value": 3467599,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 288.38ms"
          },
          {
            "name": "cylinder",
            "value": 6528651,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 153.17ms"
          },
          {
            "name": "sphere",
            "value": 2405946,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 415.64ms"
          },
          {
            "name": "torus",
            "value": 2746068,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 364.16ms"
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
          "id": "fddf48f2c39b8d1c7fe500f7b3625783af086612",
          "message": "Expand CI matrix to all 4 backends with shaded renders\n\n- Add OCCT, Embree, SIMD4, SIMD8 backend configurations for Linux\n- Update benchmark parser to accept --backend flag\n- Create render_shaded.py for Lambertian-shaded geometry renders\n- Upload rendered images to gh-pages for dashboard display\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T15:34:35Z",
          "tree_id": "19ca2878454bc724ac218699054106fa4a3985c2",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/fddf48f2c39b8d1c7fe500f7b3625783af086612"
        },
        "date": 1765640183051,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 17964287,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.67ms"
          },
          {
            "name": "bspline_surface",
            "value": 3738178,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 267.51ms"
          },
          {
            "name": "cone",
            "value": 5974822,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.37ms"
          },
          {
            "name": "cylinder",
            "value": 18128093,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.16ms"
          },
          {
            "name": "sphere",
            "value": 4945843,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.19ms"
          },
          {
            "name": "torus",
            "value": 7136383,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.13ms"
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
        "date": 1765640431067,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 18410442,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.32ms"
          },
          {
            "name": "bspline_surface",
            "value": 3910726,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.71ms"
          },
          {
            "name": "cone",
            "value": 5974786,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.37ms"
          },
          {
            "name": "cylinder",
            "value": 18641414,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 53.64ms"
          },
          {
            "name": "sphere",
            "value": 4991739,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 200.33ms"
          },
          {
            "name": "torus",
            "value": 7233064,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 138.25ms"
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
        "date": 1765640834171,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 18393511,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.37ms"
          },
          {
            "name": "bspline_surface",
            "value": 3931513,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 254.36ms"
          },
          {
            "name": "cone",
            "value": 5977286,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.30ms"
          },
          {
            "name": "cylinder",
            "value": 18429783,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.26ms"
          },
          {
            "name": "sphere",
            "value": 4946063,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.18ms"
          },
          {
            "name": "torus",
            "value": 6915964,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 144.59ms"
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
          "id": "137cb5a927ec4fec4f78fc098528c214c42d69b2",
          "message": "Fix render script to look for npy output in correct directory\n\nThe raytracer outputs to the same directory as the input file, not\nthe current working directory. Fixed the script to look in the\nbrep file's directory for the generated npy file.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T15:58:59Z",
          "tree_id": "4d9fa015fbbfc07b7039f6a3daee126e517261e1",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/137cb5a927ec4fec4f78fc098528c214c42d69b2"
        },
        "date": 1765641649182,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 12537927,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.76ms"
          },
          {
            "name": "bspline_surface",
            "value": 3438967,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 290.78ms"
          },
          {
            "name": "cone",
            "value": 5005180,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 199.79ms"
          },
          {
            "name": "cylinder",
            "value": 12789196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 78.19ms"
          },
          {
            "name": "sphere",
            "value": 4165521,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 240.07ms"
          },
          {
            "name": "torus",
            "value": 5856412,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 170.75ms"
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
          "id": "7a4aedd6cdca98982d2d838899e4199a493c0abb",
          "message": "Increase render resolution to 1024 and add mesh statistics\n\n- Update resolution from 256 to 1024 for higher quality renders\n- Add mesh statistics collection (tessellation time, triangle count)\n- Export STL to capture triangle count from raytracer output\n- Generate metadata.json with per-geometry stats\n- Add new test geometry files\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:05:23Z",
          "tree_id": "b96c77ca71f4bae07038a28e0aa9a1f6bbede18a",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/7a4aedd6cdca98982d2d838899e4199a493c0abb"
        },
        "date": 1765642116274,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7139084,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.07ms"
          },
          {
            "name": "box",
            "value": 18196044,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.96ms"
          },
          {
            "name": "bspline_surface",
            "value": 3917881,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.24ms"
          },
          {
            "name": "cone",
            "value": 5984942,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.09ms"
          },
          {
            "name": "cylinder",
            "value": 18606728,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 53.74ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2744395,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.38ms"
          },
          {
            "name": "mr",
            "value": 10835410,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.29ms"
          },
          {
            "name": "simple_L021",
            "value": 10300040,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.09ms"
          },
          {
            "name": "sphere",
            "value": 4970970,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 201.17ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3827619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.26ms"
          },
          {
            "name": "torus",
            "value": 7168048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.51ms"
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
          "id": "052942841368d952e7b29fb8c3720dcc0118f255",
          "message": "Also upload metadata.json to gh-pages with renders\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:22:21Z",
          "tree_id": "38be952309c19e209b3d41c38b541dc8af29a428",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/052942841368d952e7b29fb8c3720dcc0118f255"
        },
        "date": 1765643131287,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7055719,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.73ms"
          },
          {
            "name": "box",
            "value": 18033615,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.45ms"
          },
          {
            "name": "bspline_surface",
            "value": 3910848,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.70ms"
          },
          {
            "name": "cone",
            "value": 5931761,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.58ms"
          },
          {
            "name": "cylinder",
            "value": 18249840,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.80ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2734893,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 365.64ms"
          },
          {
            "name": "mr",
            "value": 10489657,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 95.33ms"
          },
          {
            "name": "simple_L021",
            "value": 9897658,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 101.03ms"
          },
          {
            "name": "sphere",
            "value": 4930626,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.81ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3781490,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 264.45ms"
          },
          {
            "name": "torus",
            "value": 7092500,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.99ms"
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
          "id": "29ef15f78038dd7697a287b853e6dd798b0a44ea",
          "message": "Fix Windows benchmarks by copying DLLs to build directory\n\nThe Python subprocess on Windows doesn't inherit PATH properly from\nGit Bash, so the raytracer couldn't find OCCT DLLs. Fix by:\n- Adding a step to copy TK*.dll and tbb*.dll to build/bin\n- Using PowerShell instead of Git Bash for benchmark step\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:24:22Z",
          "tree_id": "b5b76ad6a2d3a253657b753df6db1d2d2c89f76e",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/29ef15f78038dd7697a287b853e6dd798b0a44ea"
        },
        "date": 1765643251346,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7021584,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 142.42ms"
          },
          {
            "name": "box",
            "value": 18064563,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.36ms"
          },
          {
            "name": "bspline_surface",
            "value": 3924785,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 254.79ms"
          },
          {
            "name": "cone",
            "value": 5987056,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.03ms"
          },
          {
            "name": "cylinder",
            "value": 17950743,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.71ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2742649,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.61ms"
          },
          {
            "name": "mr",
            "value": 10746333,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 93.06ms"
          },
          {
            "name": "simple_L021",
            "value": 10174286,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.29ms"
          },
          {
            "name": "sphere",
            "value": 5003052,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 199.88ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3812763,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.28ms"
          },
          {
            "name": "torus",
            "value": 7192382,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.04ms"
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
          "id": "b374ee08daa913cd04a494a6fb0801c7d97d70f0",
          "message": "Add Windows raytracer debug step and fix PATH for subprocess\n\n- Add test step to run raytracer --help directly\n- Set PATH in PowerShell before running benchmarks\n- Use Windows-style paths for raytracer and test_data\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:30:42Z",
          "tree_id": "530ab58cba6e8ccd93256ed5a2ced6f2caacb5a7",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/b374ee08daa913cd04a494a6fb0801c7d97d70f0"
        },
        "date": 1765643620987,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 6896029,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 145.01ms"
          },
          {
            "name": "box",
            "value": 16997841,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 58.83ms"
          },
          {
            "name": "bspline_surface",
            "value": 3865033,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 258.73ms"
          },
          {
            "name": "cone",
            "value": 5828423,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 171.57ms"
          },
          {
            "name": "cylinder",
            "value": 16958925,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 58.97ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2724781,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 367.00ms"
          },
          {
            "name": "mr",
            "value": 10300465,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 97.08ms"
          },
          {
            "name": "simple_L021",
            "value": 9779664,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 102.25ms"
          },
          {
            "name": "sphere",
            "value": 4857364,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 205.87ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3787678,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 264.01ms"
          },
          {
            "name": "torus",
            "value": 6962382,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 143.63ms"
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
          "id": "79c72c38b1fc9f733295e6c73f543c371db1a84e",
          "message": "Add more Windows DLLs and improve raytracer debug output\n\n- Copy freetype, freeimage, tcl, zlib DLLs in addition to OCCT/TBB\n- Use dumpbin to check exe dependencies\n- Use Start-Process with output redirection for better diagnostics\n- Add continue-on-error to see benchmark step even if test fails\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:39:33Z",
          "tree_id": "028573d93738730deb27cd6b3e49d0556407a5ae",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/79c72c38b1fc9f733295e6c73f543c371db1a84e"
        },
        "date": 1765644155142,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7092752,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.99ms"
          },
          {
            "name": "box",
            "value": 18247842,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.80ms"
          },
          {
            "name": "bspline_surface",
            "value": 3928470,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 254.55ms"
          },
          {
            "name": "cone",
            "value": 5929510,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.65ms"
          },
          {
            "name": "cylinder",
            "value": 18042725,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.42ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2738211,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 365.20ms"
          },
          {
            "name": "mr",
            "value": 10796454,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.62ms"
          },
          {
            "name": "simple_L021",
            "value": 10189525,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.14ms"
          },
          {
            "name": "sphere",
            "value": 4917847,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 203.34ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3808552,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.57ms"
          },
          {
            "name": "torus",
            "value": 7162503,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.62ms"
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
          "id": "41ca429b9b358644e101579c23528a72bfff532e",
          "message": "Debug Windows: find and copy OCCT_RT.dll\n\nThe raytracer can't find OCCT_RT.dll (our library). Add debugging\nto find where CMake places it in the build directory, then copy it\nto build/bin along with other DLLs.\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T16:49:01Z",
          "tree_id": "c0207d6ff32f25e48c4621d6c576cc9af3c8a675",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/41ca429b9b358644e101579c23528a72bfff532e"
        },
        "date": 1765644718839,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7093356,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.98ms"
          },
          {
            "name": "box",
            "value": 17755682,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 56.32ms"
          },
          {
            "name": "bspline_surface",
            "value": 3928764,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 254.53ms"
          },
          {
            "name": "cone",
            "value": 5925259,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.77ms"
          },
          {
            "name": "cylinder",
            "value": 18126122,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.17ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2744365,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.38ms"
          },
          {
            "name": "mr",
            "value": 10833884,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.30ms"
          },
          {
            "name": "simple_L021",
            "value": 10293149,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.15ms"
          },
          {
            "name": "sphere",
            "value": 4991415,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 200.34ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3809132,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.53ms"
          },
          {
            "name": "torus",
            "value": 7148423,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.89ms"
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
          "id": "f328d144d970736e029332a469674c798bb8cbe1",
          "message": "Add Windows Embree/SIMD4/SIMD8 benchmark configurations\n\n- Add win-x64-embree, win-x64-simd4, win-x64-simd8 jobs to CI matrix\n- Include embree*.dll in Windows DLL copy step\n\n🤖 Generated with [Claude Code](https://claude.com/claude-code)\n\nCo-Authored-By: Claude Opus 4.5 <noreply@anthropic.com>",
          "timestamp": "2025-12-13T17:01:27Z",
          "tree_id": "2d2257f28d1e0b2b1f826e44f318d75f5e0e73af",
          "url": "https://github.com/PozzettiAndrea/OCCT-RT/commit/f328d144d970736e029332a469674c798bb8cbe1"
        },
        "date": 1765645467108,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7076991,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.30ms"
          },
          {
            "name": "box",
            "value": 18461084,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.17ms"
          },
          {
            "name": "bspline_surface",
            "value": 3924847,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 254.79ms"
          },
          {
            "name": "cone",
            "value": 5943960,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.24ms"
          },
          {
            "name": "cylinder",
            "value": 18110037,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.22ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2747140,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.01ms"
          },
          {
            "name": "mr",
            "value": 10859650,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.08ms"
          },
          {
            "name": "simple_L021",
            "value": 10180086,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.23ms"
          },
          {
            "name": "sphere",
            "value": 4964109,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 201.45ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3803467,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.92ms"
          },
          {
            "name": "torus",
            "value": 7140919,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.04ms"
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
        "date": 1765647953909,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7066339,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.52ms"
          },
          {
            "name": "box",
            "value": 18124479,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.17ms"
          },
          {
            "name": "bspline_surface",
            "value": 3915044,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.42ms"
          },
          {
            "name": "cone",
            "value": 5924346,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.80ms"
          },
          {
            "name": "cylinder",
            "value": 18302950,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.64ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2743273,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.53ms"
          },
          {
            "name": "mr",
            "value": 10693129,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 93.52ms"
          },
          {
            "name": "simple_L021",
            "value": 10134176,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.68ms"
          },
          {
            "name": "sphere",
            "value": 4937443,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.53ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3810104,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.46ms"
          },
          {
            "name": "torus",
            "value": 7157684,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.71ms"
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
        "date": 1765651083793,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7145715,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 139.94ms"
          },
          {
            "name": "box",
            "value": 18374247,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.42ms"
          },
          {
            "name": "bspline_surface",
            "value": 3950976,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 253.10ms"
          },
          {
            "name": "cone",
            "value": 5956671,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.88ms"
          },
          {
            "name": "cylinder",
            "value": 18453589,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.19ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2751607,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 363.42ms"
          },
          {
            "name": "mr",
            "value": 10852814,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.14ms"
          },
          {
            "name": "simple_L021",
            "value": 10267573,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.39ms"
          },
          {
            "name": "sphere",
            "value": 4953266,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 201.89ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3812443,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.30ms"
          },
          {
            "name": "torus",
            "value": 7211261,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 138.67ms"
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
        "date": 1765651200111,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7159016,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 139.68ms"
          },
          {
            "name": "box",
            "value": 18272850,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.73ms"
          },
          {
            "name": "bspline_surface",
            "value": 3947467,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 253.33ms"
          },
          {
            "name": "cone",
            "value": 5343108,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 187.16ms"
          },
          {
            "name": "cylinder",
            "value": 18517833,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.00ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2751592,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 363.43ms"
          },
          {
            "name": "mr",
            "value": 10774933,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.81ms"
          },
          {
            "name": "simple_L021",
            "value": 10187241,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 98.16ms"
          },
          {
            "name": "sphere",
            "value": 5000925,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 199.96ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3821432,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.68ms"
          },
          {
            "name": "torus",
            "value": 7191296,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.06ms"
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
        "date": 1765651561595,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7040221,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 142.04ms"
          },
          {
            "name": "box",
            "value": 17929501,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.77ms"
          },
          {
            "name": "bspline_surface",
            "value": 3919110,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.16ms"
          },
          {
            "name": "cone",
            "value": 5925785,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.75ms"
          },
          {
            "name": "cylinder",
            "value": 17964932,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 55.66ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2743273,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.53ms"
          },
          {
            "name": "mr",
            "value": 10707441,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 93.39ms"
          },
          {
            "name": "simple_L021",
            "value": 9948665,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 100.52ms"
          },
          {
            "name": "sphere",
            "value": 4944865,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.23ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3820979,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.71ms"
          },
          {
            "name": "torus",
            "value": 7092752,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.99ms"
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
        "date": 1765653093184,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7088127,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.08ms"
          },
          {
            "name": "box",
            "value": 18135326,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.14ms"
          },
          {
            "name": "bspline_surface",
            "value": 3913848,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 255.50ms"
          },
          {
            "name": "cone",
            "value": 5918315,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 168.97ms"
          },
          {
            "name": "cylinder",
            "value": 18541180,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 53.93ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2734048,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 365.76ms"
          },
          {
            "name": "mr",
            "value": 10715242,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 93.33ms"
          },
          {
            "name": "simple_L021",
            "value": 10265043,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.42ms"
          },
          {
            "name": "sphere",
            "value": 4929168,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.87ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3812995,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.26ms"
          },
          {
            "name": "torus",
            "value": 7188039,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 139.12ms"
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
        "date": 1765664541039,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5644169,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 177.17ms"
          },
          {
            "name": "box",
            "value": 12352999,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.95ms"
          },
          {
            "name": "bspline_surface",
            "value": 3426242,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 291.86ms"
          },
          {
            "name": "cone",
            "value": 4960613,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 201.59ms"
          },
          {
            "name": "cylinder",
            "value": 12302544,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 81.28ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2289807,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 436.72ms"
          },
          {
            "name": "mr",
            "value": 7979254,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 125.33ms"
          },
          {
            "name": "simple_L021",
            "value": 8140074,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 122.85ms"
          },
          {
            "name": "sphere",
            "value": 4244068,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 235.62ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3391716,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 294.84ms"
          },
          {
            "name": "torus",
            "value": 5931867,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.58ms"
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
        "date": 1765666784625,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7047167,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.90ms"
          },
          {
            "name": "box",
            "value": 17527869,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 57.05ms"
          },
          {
            "name": "bspline_surface",
            "value": 3871183,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 258.32ms"
          },
          {
            "name": "cone",
            "value": 5816862,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 171.91ms"
          },
          {
            "name": "cylinder",
            "value": 18223235,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.88ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2735619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 365.55ms"
          },
          {
            "name": "mr",
            "value": 10780509,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.76ms"
          },
          {
            "name": "simple_L021",
            "value": 10221395,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.83ms"
          },
          {
            "name": "sphere",
            "value": 4989970,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 200.40ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3830875,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.04ms"
          },
          {
            "name": "torus",
            "value": 7130582,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 140.24ms"
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
        "date": 1765668169937,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 12073358,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 82.83ms"
          },
          {
            "name": "box_raw",
            "value": 17690035,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 56.53ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 10979841,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 91.08ms"
          },
          {
            "name": "cone_raw",
            "value": 14636574,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 68.32ms"
          },
          {
            "name": "cylinder_raw",
            "value": 18727293,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 53.40ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 11424393,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 87.53ms"
          },
          {
            "name": "mr_raw",
            "value": 22659808,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 44.13ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 13604332,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 73.51ms"
          },
          {
            "name": "sphere_raw",
            "value": 13528688,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 73.92ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 13722880,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 72.87ms"
          },
          {
            "name": "torus_raw",
            "value": 16805875,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 59.50ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 7670829,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 130.36ms"
          },
          {
            "name": "box_refined",
            "value": 18108069,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.22ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 4349396,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 229.92ms"
          },
          {
            "name": "cone_refined",
            "value": 6223666,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 160.68ms"
          },
          {
            "name": "cylinder_refined",
            "value": 18402311,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.34ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 3189243,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 313.55ms"
          },
          {
            "name": "mr_refined",
            "value": 12016053,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 83.22ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 10393390,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 96.21ms"
          },
          {
            "name": "sphere_refined",
            "value": 5177189,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 193.15ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 4014001,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 249.13ms"
          },
          {
            "name": "torus_refined",
            "value": 7449844,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 134.23ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 7674126,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 130.31ms"
          },
          {
            "name": "box_normals",
            "value": 18128750,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 55.16ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 4377536,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 228.44ms"
          },
          {
            "name": "cone_normals",
            "value": 6228163,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 160.56ms"
          },
          {
            "name": "cylinder_normals",
            "value": 18267843,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.74ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 3167293,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 315.73ms"
          },
          {
            "name": "mr_normals",
            "value": 12049790,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 82.99ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 10388423,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 96.26ms"
          },
          {
            "name": "sphere_normals",
            "value": 5166223,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 193.56ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 4071065,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 245.64ms"
          },
          {
            "name": "torus_normals",
            "value": 7407792,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 134.99ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 7089433,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.05ms"
          },
          {
            "name": "box_curvature",
            "value": 18408748,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.32ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 3940281,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 253.79ms"
          },
          {
            "name": "cone_curvature",
            "value": 5987128,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 167.02ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 18272850,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 54.73ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2743462,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 364.50ms"
          },
          {
            "name": "mr_curvature",
            "value": 10727772,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 93.22ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 10303542,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.05ms"
          },
          {
            "name": "sphere_curvature",
            "value": 5003578,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 199.86ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 3821505,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.68ms"
          },
          {
            "name": "torus_curvature",
            "value": 7197300,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 138.94ms"
          }
        ]
      }
    ]
  }
}