window.BENCHMARK_DATA = {
  "lastUpdate": 1765645569828,
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
        "date": 1765633473757,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10544074,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 94.84ms"
          },
          {
            "name": "bspline_surface",
            "value": 2925653,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 341.80ms"
          },
          {
            "name": "cone",
            "value": 3413319,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 292.97ms"
          },
          {
            "name": "cylinder",
            "value": 6617214,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 151.12ms"
          },
          {
            "name": "sphere",
            "value": 2382075,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 419.80ms"
          },
          {
            "name": "torus",
            "value": 2695912,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 370.93ms"
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
        "date": 1765633977240,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10693129,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 93.52ms"
          },
          {
            "name": "bspline_surface",
            "value": 2941886,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 339.92ms"
          },
          {
            "name": "cone",
            "value": 3459214,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.08ms"
          },
          {
            "name": "cylinder",
            "value": 6741134,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.34ms"
          },
          {
            "name": "sphere",
            "value": 2359620,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 423.80ms"
          },
          {
            "name": "torus",
            "value": 2737236,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 365.33ms"
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
        "date": 1765634480891,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10270737,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 97.36ms"
          },
          {
            "name": "bspline_surface",
            "value": 2682792,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 372.75ms"
          },
          {
            "name": "cone",
            "value": 3381006,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 295.77ms"
          },
          {
            "name": "cylinder",
            "value": 6492410,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 154.03ms"
          },
          {
            "name": "sphere",
            "value": 2332579,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 428.71ms"
          },
          {
            "name": "torus",
            "value": 2554004,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 391.54ms"
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
        "date": 1765635055405,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10554312,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 94.75ms"
          },
          {
            "name": "bspline_surface",
            "value": 2936375,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 340.56ms"
          },
          {
            "name": "cone",
            "value": 3460232,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.00ms"
          },
          {
            "name": "cylinder",
            "value": 6719030,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.83ms"
          },
          {
            "name": "sphere",
            "value": 2389104,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 418.57ms"
          },
          {
            "name": "torus",
            "value": 2718381,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 367.87ms"
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
        "date": 1765635617237,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 9205475,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 108.63ms"
          },
          {
            "name": "bspline_surface",
            "value": 2942483,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 339.85ms"
          },
          {
            "name": "cone",
            "value": 3465388,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 288.57ms"
          },
          {
            "name": "cylinder",
            "value": 6772085,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 147.67ms"
          },
          {
            "name": "sphere",
            "value": 2412359,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 414.53ms"
          },
          {
            "name": "torus",
            "value": 2732614,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 365.95ms"
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
        "date": 1765636523279,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 10118795,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 98.83ms"
          },
          {
            "name": "bspline_surface",
            "value": 2930987,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 341.18ms"
          },
          {
            "name": "cone",
            "value": 3318136,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 301.37ms"
          },
          {
            "name": "cylinder",
            "value": 6714384,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.93ms"
          },
          {
            "name": "sphere",
            "value": 2150602,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 464.99ms"
          },
          {
            "name": "torus",
            "value": 2650158,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 377.34ms"
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
        "date": 1765641640611,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 8096314,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 123.51ms"
          },
          {
            "name": "bspline_surface",
            "value": 2607344,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 383.53ms"
          },
          {
            "name": "cone",
            "value": 2997674,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 333.59ms"
          },
          {
            "name": "cylinder",
            "value": 5347737,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 186.99ms"
          },
          {
            "name": "sphere",
            "value": 2114062,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 473.02ms"
          },
          {
            "name": "torus",
            "value": 2406658,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 415.51ms"
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
        "date": 1765642198871,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 817154,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1223.76ms"
          },
          {
            "name": "box",
            "value": 10791212,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.67ms"
          },
          {
            "name": "bspline_surface",
            "value": 2958799,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 337.98ms"
          },
          {
            "name": "cone",
            "value": 3469813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 288.20ms"
          },
          {
            "name": "cylinder",
            "value": 6743907,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.28ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1898005,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 526.87ms"
          },
          {
            "name": "mr",
            "value": 4978815,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 200.85ms"
          },
          {
            "name": "simple_L021",
            "value": 2869268,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 348.52ms"
          },
          {
            "name": "sphere",
            "value": 2391887,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 418.08ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2565076,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 389.85ms"
          },
          {
            "name": "torus",
            "value": 2636887,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 379.24ms"
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
        "date": 1765643229168,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 807950,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1237.70ms"
          },
          {
            "name": "box",
            "value": 10685016,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 93.59ms"
          },
          {
            "name": "bspline_surface",
            "value": 2937858,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 340.38ms"
          },
          {
            "name": "cone",
            "value": 3443040,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 290.44ms"
          },
          {
            "name": "cylinder",
            "value": 6661693,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 150.11ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1898200,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 526.82ms"
          },
          {
            "name": "mr",
            "value": 4892535,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 204.39ms"
          },
          {
            "name": "simple_L021",
            "value": 2814800,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 355.27ms"
          },
          {
            "name": "sphere",
            "value": 2336858,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 427.92ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2522800,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 396.38ms"
          },
          {
            "name": "torus",
            "value": 2678940,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 373.28ms"
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
        "date": 1765643346673,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 814395,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1227.90ms"
          },
          {
            "name": "box",
            "value": 10578205,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 94.53ms"
          },
          {
            "name": "bspline_surface",
            "value": 2956419,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 338.25ms"
          },
          {
            "name": "cone",
            "value": 3458807,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.12ms"
          },
          {
            "name": "cylinder",
            "value": 6728343,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.62ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1900331,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 526.22ms"
          },
          {
            "name": "mr",
            "value": 5025504,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 198.99ms"
          },
          {
            "name": "simple_L021",
            "value": 2852253,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 350.60ms"
          },
          {
            "name": "sphere",
            "value": 2401099,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 416.48ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2564149,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 389.99ms"
          },
          {
            "name": "torus",
            "value": 2734407,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 365.71ms"
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
        "date": 1765643706930,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 811786,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1231.85ms"
          },
          {
            "name": "box",
            "value": 10796688,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.62ms"
          },
          {
            "name": "bspline_surface",
            "value": 2956760,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 338.21ms"
          },
          {
            "name": "cone",
            "value": 3450203,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.84ms"
          },
          {
            "name": "cylinder",
            "value": 6760045,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 147.93ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1899310,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 526.51ms"
          },
          {
            "name": "mr",
            "value": 4926278,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 202.99ms"
          },
          {
            "name": "simple_L021",
            "value": 2854280,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 350.35ms"
          },
          {
            "name": "sphere",
            "value": 2407765,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 415.32ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2566590,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 389.62ms"
          },
          {
            "name": "torus",
            "value": 2729265,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 366.40ms"
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
        "date": 1765644240992,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 814963,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1227.05ms"
          },
          {
            "name": "box",
            "value": 10762641,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.91ms"
          },
          {
            "name": "bspline_surface",
            "value": 2946888,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 339.34ms"
          },
          {
            "name": "cone",
            "value": 3452168,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.67ms"
          },
          {
            "name": "cylinder",
            "value": 6747411,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.21ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1894542,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 527.83ms"
          },
          {
            "name": "mr",
            "value": 4944914,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 202.23ms"
          },
          {
            "name": "simple_L021",
            "value": 2820071,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 354.60ms"
          },
          {
            "name": "sphere",
            "value": 2389538,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 418.49ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2518454,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 397.07ms"
          },
          {
            "name": "torus",
            "value": 2595656,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 385.26ms"
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
        "date": 1765644810682,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 805978,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1240.73ms"
          },
          {
            "name": "box",
            "value": 10650535,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 93.89ms"
          },
          {
            "name": "bspline_surface",
            "value": 2951637,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 338.79ms"
          },
          {
            "name": "cone",
            "value": 3468176,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 288.34ms"
          },
          {
            "name": "cylinder",
            "value": 6677484,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 149.76ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1897544,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 527.00ms"
          },
          {
            "name": "mr",
            "value": 4932645,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 202.73ms"
          },
          {
            "name": "simple_L021",
            "value": 2831931,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 353.12ms"
          },
          {
            "name": "sphere",
            "value": 2377578,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 420.60ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2366612,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 422.54ms"
          },
          {
            "name": "torus",
            "value": 2687060,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 372.15ms"
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
        "date": 1765645569205,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 779502,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1282.87ms"
          },
          {
            "name": "box",
            "value": 10551751,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 94.77ms"
          },
          {
            "name": "bspline_surface",
            "value": 2943245,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 339.76ms"
          },
          {
            "name": "cone",
            "value": 3449156,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 289.93ms"
          },
          {
            "name": "cylinder",
            "value": 6718669,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 148.84ms"
          },
          {
            "name": "io1-ca-214",
            "value": 1901090,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 526.01ms"
          },
          {
            "name": "mr",
            "value": 4928026,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 202.92ms"
          },
          {
            "name": "simple_L021",
            "value": 2845711,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 351.41ms"
          },
          {
            "name": "sphere",
            "value": 2397576,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 417.09ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2552948,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 391.70ms"
          },
          {
            "name": "torus",
            "value": 2715982,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 368.19ms"
          }
        ]
      }
    ]
  }
}