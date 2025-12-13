#!/usr/bin/env python3
"""
Parse OCCT-RT raytracer benchmark output into JSON format for github-action-benchmark.

Usage: python parse_benchmark.py <raytracer_path> <test_data_dir> <output.json> [--backend BACKEND]

The raytracer outputs benchmark results in this format:
    10000 rays |     8500 hits |     123.45 ms |    2030000 rays/sec

This script runs the raytracer with -b flag on each BREP file and extracts
the final benchmark line (highest resolution) for trend tracking.
"""

import subprocess
import json
import re
import sys
import os
import argparse
from pathlib import Path


def run_benchmark(raytracer: str, brep_file: str, backend: str = "occt", resolution: int = 1000) -> dict:
    """
    Run benchmark on a single BREP file and parse output.

    Args:
        raytracer: Path to raytracer executable
        brep_file: Path to BREP file
        backend: BVH backend (occt, embree, embree4, embree8)
        resolution: Grid resolution for benchmark (default: 1000x1000 = 1M rays)

    Returns:
        Dictionary with benchmark results or None on failure
    """
    try:
        result = subprocess.run(
            [raytracer, "-b", "-r", str(resolution), "--backend", backend, brep_file],
            capture_output=True,
            text=True,
            timeout=600  # 10 minute timeout for large files
        )

        output = result.stdout + result.stderr

        # Parse benchmark lines
        # Format: "  250000 rays |   125000 hits |     123.45 ms |    2030000 rays/sec"
        pattern = r'(\d+)\s*rays\s*\|\s*(\d+)\s*hits\s*\|\s*([\d.]+)\s*ms\s*\|\s*(\d+)\s*rays/sec'
        matches = re.findall(pattern, output)

        if matches:
            # Take the last (highest resolution) benchmark result
            last_match = matches[-1]
            rays, hits, ms, rays_per_sec = last_match
            return {
                "rays": int(rays),
                "hits": int(hits),
                "time_ms": float(ms),
                "rays_per_sec": int(rays_per_sec)
            }
        else:
            print(f"  Warning: No benchmark output found for {brep_file}", file=sys.stderr)
            print(f"  Output was: {output[:500]}...", file=sys.stderr)

    except subprocess.TimeoutExpired:
        print(f"  Timeout on {brep_file} (exceeded 10 minutes)", file=sys.stderr)
    except FileNotFoundError:
        print(f"  Error: Raytracer not found at {raytracer}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"  Error on {brep_file}: {e}", file=sys.stderr)

    return None


def main():
    parser = argparse.ArgumentParser(
        description="Parse OCCT-RT raytracer benchmark output into JSON format"
    )
    parser.add_argument("raytracer", help="Path to the raytracer executable")
    parser.add_argument("test_data_dir", help="Directory containing .brep test files")
    parser.add_argument("output_json", help="Output file for benchmark results")
    parser.add_argument("--backend", default="occt", choices=["occt", "embree", "embree4", "embree8"],
                        help="BVH backend to use (default: occt)")
    parser.add_argument("--resolution", type=int, default=1000,
                        help="Benchmark resolution (default: 1000 = 1M rays)")

    args = parser.parse_args()

    raytracer = args.raytracer
    test_data_dir = Path(args.test_data_dir)
    output_file = args.output_json
    backend = args.backend
    resolution = args.resolution

    # Verify raytracer exists
    if not os.path.isfile(raytracer):
        print(f"Error: Raytracer not found at {raytracer}", file=sys.stderr)
        sys.exit(1)

    # Verify test data directory exists
    if not test_data_dir.is_dir():
        print(f"Error: Test data directory not found at {test_data_dir}", file=sys.stderr)
        sys.exit(1)

    # Find all BREP files
    brep_files = sorted(test_data_dir.glob("*.brep"))
    if not brep_files:
        print(f"Error: No .brep files found in {test_data_dir}", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(brep_files)} BREP files to benchmark")
    print(f"Raytracer: {raytracer}")
    print(f"Backend: {backend}")
    print(f"Resolution: {resolution}x{resolution} ({resolution*resolution:,} rays)")
    print()

    benchmarks = []

    for brep_file in brep_files:
        filename = brep_file.stem
        print(f"Benchmarking: {filename}...")

        result = run_benchmark(raytracer, str(brep_file), backend=backend, resolution=resolution)

        if result:
            benchmarks.append({
                "name": filename,
                "unit": "rays/sec",
                "value": result["rays_per_sec"],
                "extra": f"Rays: {result['rays']:,}, Hits: {result['hits']:,}, Time: {result['time_ms']:.2f}ms"
            })
            print(f"  -> {result['rays_per_sec']:,} rays/sec")
        else:
            print(f"  -> FAILED")

    # Write output JSON
    with open(output_file, 'w') as f:
        json.dump(benchmarks, f, indent=2)

    print()
    print(f"Wrote {len(benchmarks)} benchmark results to {output_file}")

    # Exit with error if no benchmarks succeeded
    if not benchmarks:
        print("Error: No benchmarks completed successfully", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
