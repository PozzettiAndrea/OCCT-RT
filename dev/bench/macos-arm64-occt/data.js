window.BENCHMARK_DATA = {
  "lastUpdate": 1765671366793,
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
        "date": 1765633488280,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 14115920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 70.84ms"
          },
          {
            "name": "bspline_surface",
            "value": 5543606,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 180.39ms"
          },
          {
            "name": "cone",
            "value": 5359459,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 186.59ms"
          },
          {
            "name": "cylinder",
            "value": 10093771,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 99.07ms"
          },
          {
            "name": "sphere",
            "value": 3736446,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 267.63ms"
          },
          {
            "name": "torus",
            "value": 4143858,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 241.32ms"
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
        "date": 1765633976600,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19645593,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.90ms"
          },
          {
            "name": "bspline_surface",
            "value": 5523672,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 181.04ms"
          },
          {
            "name": "cone",
            "value": 5789885,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 172.71ms"
          },
          {
            "name": "cylinder",
            "value": 11675696,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 85.65ms"
          },
          {
            "name": "sphere",
            "value": 3365043,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 297.17ms"
          },
          {
            "name": "torus",
            "value": 4228026,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 236.52ms"
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
        "date": 1765634476010,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 17310921,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 57.77ms"
          },
          {
            "name": "bspline_surface",
            "value": 5491880,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 182.09ms"
          },
          {
            "name": "cone",
            "value": 4058639,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 246.39ms"
          },
          {
            "name": "cylinder",
            "value": 9365839,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 106.77ms"
          },
          {
            "name": "sphere",
            "value": 2569624,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 389.16ms"
          },
          {
            "name": "torus",
            "value": 2757708,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 362.62ms"
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
        "date": 1765635106697,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 12444931,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 80.35ms"
          },
          {
            "name": "bspline_surface",
            "value": 4567879,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 218.92ms"
          },
          {
            "name": "cone",
            "value": 4521613,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 221.16ms"
          },
          {
            "name": "cylinder",
            "value": 9313849,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 107.37ms"
          },
          {
            "name": "sphere",
            "value": 2850587,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 350.81ms"
          },
          {
            "name": "torus",
            "value": 2686302,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 372.26ms"
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
        "date": 1765635616268,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 15951507,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 62.69ms"
          },
          {
            "name": "bspline_surface",
            "value": 4082366,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 244.96ms"
          },
          {
            "name": "cone",
            "value": 4462433,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 224.09ms"
          },
          {
            "name": "cylinder",
            "value": 9704216,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 103.05ms"
          },
          {
            "name": "sphere",
            "value": 3624870,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 275.87ms"
          },
          {
            "name": "torus",
            "value": 4099184,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 243.95ms"
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
        "date": 1765636532527,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 18276524,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.71ms"
          },
          {
            "name": "bspline_surface",
            "value": 5646336,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 177.11ms"
          },
          {
            "name": "cone",
            "value": 5569138,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 179.56ms"
          },
          {
            "name": "cylinder",
            "value": 11018191,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 90.76ms"
          },
          {
            "name": "sphere",
            "value": 3663648,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 272.95ms"
          },
          {
            "name": "torus",
            "value": 2721214,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 367.48ms"
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
        "date": 1765641656623,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19100008,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.36ms"
          },
          {
            "name": "bspline_surface",
            "value": 5220460,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 191.55ms"
          },
          {
            "name": "cone",
            "value": 4740168,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 210.96ms"
          },
          {
            "name": "cylinder",
            "value": 8815699,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 113.43ms"
          },
          {
            "name": "sphere",
            "value": 3397974,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 294.29ms"
          },
          {
            "name": "torus",
            "value": 4078287,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 245.20ms"
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
        "date": 1765642172509,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 1019998,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 980.39ms"
          },
          {
            "name": "box",
            "value": 19079600,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.41ms"
          },
          {
            "name": "bspline_surface",
            "value": 5813920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 172.00ms"
          },
          {
            "name": "cone",
            "value": 5372214,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 186.14ms"
          },
          {
            "name": "cylinder",
            "value": 11665345,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 85.72ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4057536,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 246.45ms"
          },
          {
            "name": "mr",
            "value": 5941947,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 168.29ms"
          },
          {
            "name": "simple_L021",
            "value": 4161188,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 240.32ms"
          },
          {
            "name": "sphere",
            "value": 2580952,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 387.45ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3468958,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 288.27ms"
          },
          {
            "name": "torus",
            "value": 3300461,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 302.99ms"
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
        "date": 1765643204026,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 741510,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1348.60ms"
          },
          {
            "name": "box",
            "value": 15351079,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 65.14ms"
          },
          {
            "name": "bspline_surface",
            "value": 4144494,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 241.28ms"
          },
          {
            "name": "cone",
            "value": 3711016,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 269.47ms"
          },
          {
            "name": "cylinder",
            "value": 9832262,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 101.71ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2786191,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 358.91ms"
          },
          {
            "name": "mr",
            "value": 6799067,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 147.08ms"
          },
          {
            "name": "simple_L021",
            "value": 5557562,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 179.93ms"
          },
          {
            "name": "sphere",
            "value": 3291238,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 303.84ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4691664,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 213.14ms"
          },
          {
            "name": "torus",
            "value": 3758862,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 266.04ms"
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
        "date": 1765643345819,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 988728,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1011.40ms"
          },
          {
            "name": "box",
            "value": 19663363,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.86ms"
          },
          {
            "name": "bspline_surface",
            "value": 5218662,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 191.62ms"
          },
          {
            "name": "cone",
            "value": 5339598,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 187.28ms"
          },
          {
            "name": "cylinder",
            "value": 11074565,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 90.30ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4074581,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 245.42ms"
          },
          {
            "name": "mr",
            "value": 5585469,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 179.04ms"
          },
          {
            "name": "simple_L021",
            "value": 4076408,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 245.31ms"
          },
          {
            "name": "sphere",
            "value": 2727970,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 366.57ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4673836,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 213.96ms"
          },
          {
            "name": "torus",
            "value": 3699976,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 270.27ms"
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
        "date": 1765643684496,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 1028106,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 972.66ms"
          },
          {
            "name": "box",
            "value": 19549196,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.15ms"
          },
          {
            "name": "bspline_surface",
            "value": 5668966,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 176.40ms"
          },
          {
            "name": "cone",
            "value": 5626037,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 177.74ms"
          },
          {
            "name": "cylinder",
            "value": 11135361,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.80ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3613853,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 276.71ms"
          },
          {
            "name": "mr",
            "value": 8516147,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 117.42ms"
          },
          {
            "name": "simple_L021",
            "value": 6015436,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 166.24ms"
          },
          {
            "name": "sphere",
            "value": 3630357,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 275.45ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4770856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 209.61ms"
          },
          {
            "name": "torus",
            "value": 4153893,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 240.74ms"
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
        "date": 1765644223648,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 712795,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1402.93ms"
          },
          {
            "name": "box",
            "value": 17657238,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 56.63ms"
          },
          {
            "name": "bspline_surface",
            "value": 5774439,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 173.18ms"
          },
          {
            "name": "cone",
            "value": 5433159,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 184.05ms"
          },
          {
            "name": "cylinder",
            "value": 11376823,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 87.90ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4027727,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 248.28ms"
          },
          {
            "name": "mr",
            "value": 7643624,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 130.83ms"
          },
          {
            "name": "simple_L021",
            "value": 5742242,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 174.15ms"
          },
          {
            "name": "sphere",
            "value": 3528606,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 283.40ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4706680,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 212.46ms"
          },
          {
            "name": "torus",
            "value": 4140307,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 241.53ms"
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
        "date": 1765644788154,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 848601,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1178.41ms"
          },
          {
            "name": "box",
            "value": 11799828,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 84.75ms"
          },
          {
            "name": "bspline_surface",
            "value": 4096933,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 244.09ms"
          },
          {
            "name": "cone",
            "value": 3020984,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 331.02ms"
          },
          {
            "name": "cylinder",
            "value": 8581849,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 116.52ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3113209,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 321.21ms"
          },
          {
            "name": "mr",
            "value": 4841865,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 206.53ms"
          },
          {
            "name": "simple_L021",
            "value": 4182805,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 239.07ms"
          },
          {
            "name": "sphere",
            "value": 2468983,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 405.02ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4591453,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 217.80ms"
          },
          {
            "name": "torus",
            "value": 4089010,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 244.56ms"
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
        "date": 1765645691093,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 633135,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1579.44ms"
          },
          {
            "name": "box",
            "value": 13962385,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 71.62ms"
          },
          {
            "name": "bspline_surface",
            "value": 4162279,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 240.25ms"
          },
          {
            "name": "cone",
            "value": 3947405,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 253.33ms"
          },
          {
            "name": "cylinder",
            "value": 7519475,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 132.99ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2949304,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 339.06ms"
          },
          {
            "name": "mr",
            "value": 3939986,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 253.81ms"
          },
          {
            "name": "simple_L021",
            "value": 3155301,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 316.93ms"
          },
          {
            "name": "sphere",
            "value": 1934154,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 517.02ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2903482,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 344.41ms"
          },
          {
            "name": "torus",
            "value": 1784271,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 560.45ms"
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
        "date": 1765648036514,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 992913,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1007.14ms"
          },
          {
            "name": "box",
            "value": 18351991,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.49ms"
          },
          {
            "name": "bspline_surface",
            "value": 5637614,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 177.38ms"
          },
          {
            "name": "cone",
            "value": 5196452,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 192.44ms"
          },
          {
            "name": "cylinder",
            "value": 11422436,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 87.55ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4019923,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 248.76ms"
          },
          {
            "name": "mr",
            "value": 7762348,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 128.83ms"
          },
          {
            "name": "simple_L021",
            "value": 5327338,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 187.71ms"
          },
          {
            "name": "sphere",
            "value": 3327920,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 300.49ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3376428,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 296.17ms"
          },
          {
            "name": "torus",
            "value": 2411085,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 414.75ms"
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
        "date": 1765651256808,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 1135000,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 881.06ms"
          },
          {
            "name": "box",
            "value": 17740877,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 56.37ms"
          },
          {
            "name": "bspline_surface",
            "value": 4181144,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 239.17ms"
          },
          {
            "name": "cone",
            "value": 4467636,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 223.83ms"
          },
          {
            "name": "cylinder",
            "value": 10008207,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 99.92ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2825346,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 353.94ms"
          },
          {
            "name": "mr",
            "value": 8576624,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 116.60ms"
          },
          {
            "name": "simple_L021",
            "value": 5764619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 173.47ms"
          },
          {
            "name": "sphere",
            "value": 3796133,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 263.43ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4600345,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 217.37ms"
          },
          {
            "name": "torus",
            "value": 4246591,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 235.48ms"
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
        "date": 1765651288021,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 656434,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1523.38ms"
          },
          {
            "name": "box",
            "value": 8173073,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 122.35ms"
          },
          {
            "name": "bspline_surface",
            "value": 3039440,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 329.01ms"
          },
          {
            "name": "cone",
            "value": 3127238,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 319.77ms"
          },
          {
            "name": "cylinder",
            "value": 4749827,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 210.53ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2280642,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 438.47ms"
          },
          {
            "name": "mr",
            "value": 2820222,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 354.58ms"
          },
          {
            "name": "simple_L021",
            "value": 2150866,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 464.93ms"
          },
          {
            "name": "sphere",
            "value": 1502797,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 665.43ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 2150760,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 464.95ms"
          },
          {
            "name": "torus",
            "value": 2598915,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 384.78ms"
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
        "date": 1765651504715,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 1050159,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 952.24ms"
          },
          {
            "name": "box",
            "value": 19218203,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.03ms"
          },
          {
            "name": "bspline_surface",
            "value": 5883772,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 169.96ms"
          },
          {
            "name": "cone",
            "value": 5696934,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 175.53ms"
          },
          {
            "name": "cylinder",
            "value": 10752457,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 93.00ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3998497,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 250.09ms"
          },
          {
            "name": "mr",
            "value": 5590777,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 178.87ms"
          },
          {
            "name": "simple_L021",
            "value": 2017919,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 495.56ms"
          },
          {
            "name": "sphere",
            "value": 3646481,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 274.24ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4843741,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 206.45ms"
          },
          {
            "name": "torus",
            "value": 4073087,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 245.51ms"
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
        "date": 1765651892545,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 696388,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1435.98ms"
          },
          {
            "name": "box",
            "value": 8718396,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 114.70ms"
          },
          {
            "name": "bspline_surface",
            "value": 2086385,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 479.30ms"
          },
          {
            "name": "cone",
            "value": 3440339,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 290.67ms"
          },
          {
            "name": "cylinder",
            "value": 7031657,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 142.21ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3504444,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 285.35ms"
          },
          {
            "name": "mr",
            "value": 8527403,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 117.27ms"
          },
          {
            "name": "simple_L021",
            "value": 6005609,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 166.51ms"
          },
          {
            "name": "sphere",
            "value": 3701976,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 270.13ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4257856,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 234.86ms"
          },
          {
            "name": "torus",
            "value": 3869729,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 258.42ms"
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
        "date": 1765653165701,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 744092,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1343.92ms"
          },
          {
            "name": "box",
            "value": 12540915,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 79.74ms"
          },
          {
            "name": "bspline_surface",
            "value": 4271186,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 234.13ms"
          },
          {
            "name": "cone",
            "value": 5591278,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 178.85ms"
          },
          {
            "name": "cylinder",
            "value": 10177806,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 98.25ms"
          },
          {
            "name": "io1-ca-214",
            "value": 3741241,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 267.29ms"
          },
          {
            "name": "mr",
            "value": 6272817,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 159.42ms"
          },
          {
            "name": "simple_L021",
            "value": 4353258,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 229.71ms"
          },
          {
            "name": "sphere",
            "value": 2326469,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 429.84ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4763992,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 209.91ms"
          },
          {
            "name": "torus",
            "value": 3844779,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 260.09ms"
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
        "date": 1765664615423,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 1082706,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 923.61ms"
          },
          {
            "name": "box",
            "value": 19474576,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.35ms"
          },
          {
            "name": "bspline_surface",
            "value": 5788243,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 172.76ms"
          },
          {
            "name": "cone",
            "value": 5755694,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 173.74ms"
          },
          {
            "name": "cylinder",
            "value": 11780228,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 84.89ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4047780,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 247.05ms"
          },
          {
            "name": "mr",
            "value": 8335765,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 119.96ms"
          },
          {
            "name": "simple_L021",
            "value": 5906884,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 169.29ms"
          },
          {
            "name": "sphere",
            "value": 3737172,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 267.58ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 4437916,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 225.33ms"
          },
          {
            "name": "torus",
            "value": 4258291,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 234.84ms"
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
        "date": 1765666853491,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 965184,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1036.07ms"
          },
          {
            "name": "box",
            "value": 19009961,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.60ms"
          },
          {
            "name": "bspline_surface",
            "value": 5394762,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 185.37ms"
          },
          {
            "name": "cone",
            "value": 5273650,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 189.62ms"
          },
          {
            "name": "cylinder",
            "value": 10925977,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 91.52ms"
          },
          {
            "name": "io1-ca-214",
            "value": 4060073,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 246.30ms"
          },
          {
            "name": "mr",
            "value": 7727916,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 129.40ms"
          },
          {
            "name": "simple_L021",
            "value": 5638886,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 177.34ms"
          },
          {
            "name": "sphere",
            "value": 2442945,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 409.34ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3473633,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 287.88ms"
          },
          {
            "name": "torus",
            "value": 2536294,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 394.28ms"
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
        "date": 1765668492838,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 1013359,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 986.82ms"
          },
          {
            "name": "box_raw",
            "value": 17496894,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 57.15ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 10427855,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 95.90ms"
          },
          {
            "name": "cone_raw",
            "value": 8599561,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 116.28ms"
          },
          {
            "name": "cylinder_raw",
            "value": 10424594,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 95.93ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 8308201,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 120.36ms"
          },
          {
            "name": "mr_raw",
            "value": 6058770,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 165.05ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 4804366,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 208.14ms"
          },
          {
            "name": "sphere_raw",
            "value": 4220228,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 236.95ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 5175957,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 193.20ms"
          },
          {
            "name": "torus_raw",
            "value": 4155343,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 240.65ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 1039269,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 962.22ms"
          },
          {
            "name": "box_refined",
            "value": 15565898,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 64.24ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 5757251,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 173.69ms"
          },
          {
            "name": "cone_refined",
            "value": 5284630,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 189.23ms"
          },
          {
            "name": "cylinder_refined",
            "value": 11127308,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.87ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 4282674,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 233.50ms"
          },
          {
            "name": "mr_refined",
            "value": 5631963,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 177.56ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 4540872,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 220.22ms"
          },
          {
            "name": "sphere_refined",
            "value": 2560059,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 390.62ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 4716803,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 212.01ms"
          },
          {
            "name": "torus_refined",
            "value": 4062217,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 246.17ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 928156,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1077.40ms"
          },
          {
            "name": "box_normals",
            "value": 11061580,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 90.40ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 4111098,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 243.24ms"
          },
          {
            "name": "cone_normals",
            "value": 3864719,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 258.75ms"
          },
          {
            "name": "cylinder_normals",
            "value": 7505254,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 133.24ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 2961077,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 337.71ms"
          },
          {
            "name": "mr_normals",
            "value": 7870359,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 127.06ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 5823027,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 171.73ms"
          },
          {
            "name": "sphere_normals",
            "value": 3645391,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 274.32ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 4693932,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 213.04ms"
          },
          {
            "name": "torus_normals",
            "value": 3739954,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 267.38ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 905622,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1104.21ms"
          },
          {
            "name": "box_curvature",
            "value": 13004070,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 76.90ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 4482094,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 223.11ms"
          },
          {
            "name": "cone_curvature",
            "value": 4290649,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 233.07ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 8711788,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 114.79ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2926398,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 341.72ms"
          },
          {
            "name": "mr_curvature",
            "value": 8095200,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 123.53ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 5533820,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 180.71ms"
          },
          {
            "name": "sphere_curvature",
            "value": 3540500,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 282.45ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 4399724,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 227.29ms"
          },
          {
            "name": "torus_curvature",
            "value": 3834562,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 260.79ms"
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
        "date": 1765671364689,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe_raw",
            "value": 1075269,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 930.00ms"
          },
          {
            "name": "box_raw",
            "value": 19129603,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.27ms"
          },
          {
            "name": "bspline_surface_raw",
            "value": 9784353,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 102.20ms"
          },
          {
            "name": "cone_raw",
            "value": 10063602,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 99.37ms"
          },
          {
            "name": "cylinder_raw",
            "value": 11460924,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 87.25ms"
          },
          {
            "name": "io1-ca-214_raw",
            "value": 8304268,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 120.42ms"
          },
          {
            "name": "mr_raw",
            "value": 8134181,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 122.94ms"
          },
          {
            "name": "simple_L021_raw",
            "value": 5866720,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 170.45ms"
          },
          {
            "name": "sphere_raw",
            "value": 5642258,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 177.23ms"
          },
          {
            "name": "spiral_wind_turbine_raw",
            "value": 8009355,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 124.85ms"
          },
          {
            "name": "torus_raw",
            "value": 3529030,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 283.36ms"
          },
          {
            "name": "bellow_pipe_refined",
            "value": 949195,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1053.52ms"
          },
          {
            "name": "box_refined",
            "value": 18787457,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 53.23ms"
          },
          {
            "name": "bspline_surface_refined",
            "value": 6015797,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 166.23ms"
          },
          {
            "name": "cone_refined",
            "value": 5267289,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 189.85ms"
          },
          {
            "name": "cylinder_refined",
            "value": 11205737,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.24ms"
          },
          {
            "name": "io1-ca-214_refined",
            "value": 4252894,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 235.13ms"
          },
          {
            "name": "mr_refined",
            "value": 5673501,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 176.26ms"
          },
          {
            "name": "simple_L021_refined",
            "value": 4212158,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 237.41ms"
          },
          {
            "name": "sphere_refined",
            "value": 2396972,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 417.19ms"
          },
          {
            "name": "spiral_wind_turbine_refined",
            "value": 3777405,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 264.73ms"
          },
          {
            "name": "torus_refined",
            "value": 4195687,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 238.34ms"
          },
          {
            "name": "bellow_pipe_normals",
            "value": 923812,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1082.47ms"
          },
          {
            "name": "box_normals",
            "value": 18584942,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 53.81ms"
          },
          {
            "name": "bspline_surface_normals",
            "value": 5970007,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 167.50ms"
          },
          {
            "name": "cone_normals",
            "value": 5591872,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 178.83ms"
          },
          {
            "name": "cylinder_normals",
            "value": 11185057,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 89.40ms"
          },
          {
            "name": "io1-ca-214_normals",
            "value": 4019842,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 248.77ms"
          },
          {
            "name": "mr_normals",
            "value": 8614600,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 116.08ms"
          },
          {
            "name": "simple_L021_normals",
            "value": 5897304,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 169.57ms"
          },
          {
            "name": "sphere_normals",
            "value": 3693008,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 270.78ms"
          },
          {
            "name": "spiral_wind_turbine_normals",
            "value": 4889545,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 204.52ms"
          },
          {
            "name": "torus_normals",
            "value": 4190149,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 238.66ms"
          },
          {
            "name": "bellow_pipe_curvature",
            "value": 965641,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 1035.58ms"
          },
          {
            "name": "box_curvature",
            "value": 18188101,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 54.98ms"
          },
          {
            "name": "bspline_surface_curvature",
            "value": 5651633,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 176.94ms"
          },
          {
            "name": "cone_curvature",
            "value": 4148310,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 241.06ms"
          },
          {
            "name": "cylinder_curvature",
            "value": 6994817,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 142.96ms"
          },
          {
            "name": "io1-ca-214_curvature",
            "value": 2592131,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 385.78ms"
          },
          {
            "name": "mr_curvature",
            "value": 8414888,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 118.84ms"
          },
          {
            "name": "simple_L021_curvature",
            "value": 5968617,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 167.54ms"
          },
          {
            "name": "sphere_curvature",
            "value": 3590510,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 278.51ms"
          },
          {
            "name": "spiral_wind_turbine_curvature",
            "value": 4489761,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 222.73ms"
          },
          {
            "name": "torus_curvature",
            "value": 4035219,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 247.82ms"
          }
        ]
      }
    ]
  }
}