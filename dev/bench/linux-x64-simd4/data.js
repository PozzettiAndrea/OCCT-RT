window.BENCHMARK_DATA = {
  "lastUpdate": 1765651201139,
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
        "date": 1765640829517,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 13299995,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 75.19ms"
          },
          {
            "name": "bspline_surface",
            "value": 3450632,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 289.80ms"
          },
          {
            "name": "cone",
            "value": 4967019,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 201.33ms"
          },
          {
            "name": "cylinder",
            "value": 13516619,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 73.98ms"
          },
          {
            "name": "sphere",
            "value": 4198576,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 238.18ms"
          },
          {
            "name": "torus",
            "value": 5926734,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 168.73ms"
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
        "date": 1765641651876,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "box",
            "value": 19548813,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.15ms"
          },
          {
            "name": "bspline_surface",
            "value": 3981526,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.16ms"
          },
          {
            "name": "cone",
            "value": 6036387,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 165.66ms"
          },
          {
            "name": "cylinder",
            "value": 19554547,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.14ms"
          },
          {
            "name": "sphere",
            "value": 4941957,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 202.35ms"
          },
          {
            "name": "torus",
            "value": 7273150,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 137.49ms"
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
        "date": 1765642118086,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7048309,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 141.88ms"
          },
          {
            "name": "box",
            "value": 19165900,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 52.18ms"
          },
          {
            "name": "bspline_surface",
            "value": 3971642,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.78ms"
          },
          {
            "name": "cone",
            "value": 6087020,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.28ms"
          },
          {
            "name": "cylinder",
            "value": 19371586,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.62ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2759001,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 362.45ms"
          },
          {
            "name": "mr",
            "value": 10768204,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.87ms"
          },
          {
            "name": "simple_L021",
            "value": 10408210,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 96.08ms"
          },
          {
            "name": "sphere",
            "value": 5040983,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 198.37ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3847589,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 259.90ms"
          },
          {
            "name": "torus",
            "value": 7261057,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 137.72ms"
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
        "date": 1765643122902,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7117134,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.51ms"
          },
          {
            "name": "box",
            "value": 19925479,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.19ms"
          },
          {
            "name": "bspline_surface",
            "value": 3985318,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 250.92ms"
          },
          {
            "name": "cone",
            "value": 6116283,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 163.50ms"
          },
          {
            "name": "cylinder",
            "value": 19368959,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.63ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2777693,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 360.01ms"
          },
          {
            "name": "mr",
            "value": 10916077,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 91.61ms"
          },
          {
            "name": "simple_L021",
            "value": 10506409,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 95.18ms"
          },
          {
            "name": "sphere",
            "value": 5082825,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 196.74ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3864824,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 258.74ms"
          },
          {
            "name": "torus",
            "value": 7305456,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 136.88ms"
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
        "date": 1765643249925,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7014098,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 142.57ms"
          },
          {
            "name": "box",
            "value": 18565965,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 53.86ms"
          },
          {
            "name": "bspline_surface",
            "value": 3955227,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 252.83ms"
          },
          {
            "name": "cone",
            "value": 6026747,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 165.93ms"
          },
          {
            "name": "cylinder",
            "value": 19261885,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.92ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2762828,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 361.95ms"
          },
          {
            "name": "mr",
            "value": 10766696,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.88ms"
          },
          {
            "name": "simple_L021",
            "value": 10437760,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 95.81ms"
          },
          {
            "name": "sphere",
            "value": 5025025,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 199.00ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3815352,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 262.10ms"
          },
          {
            "name": "torus",
            "value": 7227993,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 138.35ms"
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
        "date": 1765643606133,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5907652,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 169.27ms"
          },
          {
            "name": "box",
            "value": 13637355,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 73.33ms"
          },
          {
            "name": "bspline_surface",
            "value": 3524304,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 283.74ms"
          },
          {
            "name": "cone",
            "value": 5140437,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 194.54ms"
          },
          {
            "name": "cylinder",
            "value": 13315579,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 75.10ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2320369,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 430.97ms"
          },
          {
            "name": "mr",
            "value": 8244907,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 121.29ms"
          },
          {
            "name": "simple_L021",
            "value": 8360086,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 119.62ms"
          },
          {
            "name": "sphere",
            "value": 4357602,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 229.48ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3477438,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 287.57ms"
          },
          {
            "name": "torus",
            "value": 6067593,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 164.81ms"
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
        "date": 1765644144694,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 5292545,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 188.94ms"
          },
          {
            "name": "box",
            "value": 10792959,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 92.65ms"
          },
          {
            "name": "bspline_surface",
            "value": 3278431,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 305.02ms"
          },
          {
            "name": "cone",
            "value": 4568171,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 218.91ms"
          },
          {
            "name": "cylinder",
            "value": 10860122,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 92.08ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2207910,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 452.92ms"
          },
          {
            "name": "mr",
            "value": 7091897,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 141.01ms"
          },
          {
            "name": "simple_L021",
            "value": 7233430,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 138.25ms"
          },
          {
            "name": "sphere",
            "value": 3947155,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 253.35ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3273751,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 305.46ms"
          },
          {
            "name": "torus",
            "value": 5428941,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 184.20ms"
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
        "date": 1765644724055,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 6953184,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 143.82ms"
          },
          {
            "name": "box",
            "value": 18588743,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 53.80ms"
          },
          {
            "name": "bspline_surface",
            "value": 3984207,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 250.99ms"
          },
          {
            "name": "cone",
            "value": 6083798,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.37ms"
          },
          {
            "name": "cylinder",
            "value": 19517908,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.24ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2775727,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 360.27ms"
          },
          {
            "name": "mr",
            "value": 10786090,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.71ms"
          },
          {
            "name": "simple_L021",
            "value": 10443646,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 95.75ms"
          },
          {
            "name": "sphere",
            "value": 4981469,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 200.74ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3846539,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 259.97ms"
          },
          {
            "name": "torus",
            "value": 7266597,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 137.62ms"
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
        "date": 1765645486232,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 6945795,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 143.97ms"
          },
          {
            "name": "box",
            "value": 19534302,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.19ms"
          },
          {
            "name": "bspline_surface",
            "value": 3978199,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.37ms"
          },
          {
            "name": "cone",
            "value": 6092434,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.14ms"
          },
          {
            "name": "cylinder",
            "value": 19533921,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.19ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2764027,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 361.79ms"
          },
          {
            "name": "mr",
            "value": 10828957,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.34ms"
          },
          {
            "name": "simple_L021",
            "value": 10391554,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 96.23ms"
          },
          {
            "name": "sphere",
            "value": 4966230,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 201.36ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3671328,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 272.38ms"
          },
          {
            "name": "torus",
            "value": 7283162,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 137.30ms"
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
        "date": 1765647962125,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7009673,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 142.66ms"
          },
          {
            "name": "box",
            "value": 19440880,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 51.44ms"
          },
          {
            "name": "bspline_surface",
            "value": 3969010,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.95ms"
          },
          {
            "name": "cone",
            "value": 6111610,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 163.62ms"
          },
          {
            "name": "cylinder",
            "value": 19544611,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.17ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2777500,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 360.04ms"
          },
          {
            "name": "mr",
            "value": 10757778,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.96ms"
          },
          {
            "name": "simple_L021",
            "value": 10569819,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 94.61ms"
          },
          {
            "name": "sphere",
            "value": 5054768,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 197.83ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3845326,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 260.06ms"
          },
          {
            "name": "torus",
            "value": 7337403,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 136.29ms"
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
        "date": 1765651093233,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7127889,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.29ms"
          },
          {
            "name": "box",
            "value": 19630938,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.94ms"
          },
          {
            "name": "bspline_surface",
            "value": 3977756,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.40ms"
          },
          {
            "name": "cone",
            "value": 6062002,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 164.96ms"
          },
          {
            "name": "cylinder",
            "value": 19390743,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.57ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2767844,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 361.29ms"
          },
          {
            "name": "mr",
            "value": 10970085,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 91.16ms"
          },
          {
            "name": "simple_L021",
            "value": 10284045,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.24ms"
          },
          {
            "name": "sphere",
            "value": 5057069,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 197.74ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3735595,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 267.69ms"
          },
          {
            "name": "torus",
            "value": 7329927,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 136.43ms"
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
        "date": 1765651200326,
        "tool": "customBiggerIsBetter",
        "benches": [
          {
            "name": "bellow_pipe",
            "value": 7112274,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 492,198, Time: 140.60ms"
          },
          {
            "name": "box",
            "value": 19763236,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 50.60ms"
          },
          {
            "name": "bspline_surface",
            "value": 3980337,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 824,464, Time: 251.24ms"
          },
          {
            "name": "cone",
            "value": 6119202,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 163.42ms"
          },
          {
            "name": "cylinder",
            "value": 19402406,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,540, Time: 51.54ms"
          },
          {
            "name": "io1-ca-214",
            "value": 2777030,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 621,066, Time: 360.10ms"
          },
          {
            "name": "mr",
            "value": 10834706,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 134,219, Time: 92.30ms"
          },
          {
            "name": "simple_L021",
            "value": 10247477,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 496,400, Time: 97.59ms"
          },
          {
            "name": "sphere",
            "value": 5035906,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 647,632, Time: 198.57ms"
          },
          {
            "name": "spiral_wind_turbine",
            "value": 3827883,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 441,594, Time: 261.24ms"
          },
          {
            "name": "torus",
            "value": 7317109,
            "unit": "rays/sec",
            "extra": "Rays: 1,000,000, Hits: 438,558, Time: 136.67ms"
          }
        ]
      }
    ]
  }
}