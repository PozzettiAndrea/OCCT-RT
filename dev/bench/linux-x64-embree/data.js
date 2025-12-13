window.BENCHMARK_DATA = {
  "lastUpdate": 1765643131884,
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
      }
    ]
  }
}