window.BENCHMARK_DATA = {
  "lastUpdate": 1765642199917,
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
      }
    ]
  }
}