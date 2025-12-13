#!/usr/bin/env python3
"""
Generate Lambertian-shaded renders from OCCT-RT raytracer normal output.

Usage: python render_shaded.py <raytracer> <test_data_dir> <output_dir> [--resolution N]

Runs the raytracer on each BREP file to get normals, then computes
Lambertian shading with a top-right-front light for a clean clay look.
Also captures mesh statistics (tessellation time, triangle count).
"""

import subprocess
import numpy as np
import os
import sys
import re
import json
import argparse
from pathlib import Path

try:
    from PIL import Image
except ImportError:
    print("Error: PIL/Pillow not installed. Run: pip install pillow", file=sys.stderr)
    sys.exit(1)


def parse_mesh_stats(stdout: str) -> dict:
    """Extract mesh statistics from raytracer stdout."""
    stats = {}

    # Parse tessellation time: "Tessellation completed in X.XX ms"
    tess_match = re.search(r"Tessellation completed in ([\d.]+) ms", stdout)
    if tess_match:
        stats["tessellation_time_ms"] = float(tess_match.group(1))

    # Parse triangle count: "Wrote N triangles to STL"
    tri_match = re.search(r"Wrote (\d+) triangles to STL", stdout)
    if tri_match:
        stats["triangle_count"] = int(tri_match.group(1))

    # Parse face count: "Number of faces: N"
    face_match = re.search(r"Number of faces: (\d+)", stdout)
    if face_match:
        stats["cad_face_count"] = int(face_match.group(1))

    # Parse face types: "Face types: N plane, N cylinder, N cone, N sphere, N torus, N bspline, N bezier, N other"
    face_types_match = re.search(
        r"Face types: (\d+) plane, (\d+) cylinder, (\d+) cone, (\d+) sphere, "
        r"(\d+) torus, (\d+) bspline, (\d+) bezier, (\d+) other",
        stdout
    )
    if face_types_match:
        stats["face_types"] = {
            "plane": int(face_types_match.group(1)),
            "cylinder": int(face_types_match.group(2)),
            "cone": int(face_types_match.group(3)),
            "sphere": int(face_types_match.group(4)),
            "torus": int(face_types_match.group(5)),
            "bspline": int(face_types_match.group(6)),
            "bezier": int(face_types_match.group(7)),
            "other": int(face_types_match.group(8)),
        }

    return stats


def render_shaded(raytracer: str, brep_file: str, output_png: str, resolution: int = 256) -> tuple:
    """
    Render a BREP file with Lambertian shading.

    Args:
        raytracer: Path to raytracer executable
        brep_file: Path to BREP file
        output_png: Path to output PNG file
        resolution: Image resolution

    Returns:
        Tuple of (success: bool, stats: dict)
    """
    # Raytracer outputs to the same directory as the input file
    brep_path = Path(brep_file).resolve()
    brep_dir = brep_path.parent
    brep_stem = brep_path.stem
    npy_file = brep_dir / f"{brep_stem}_data.npy"
    stl_file = brep_dir / f"{brep_stem}_tessellation.stl"

    try:
        # Run raytracer to get normals and positions
        # -s: export STL to get triangle count
        # --allow-disconnected: some test shapes have multiple disconnected components
        result = subprocess.run(
            [raytracer, "-r", str(resolution), "--normals", "--position",
             "-s", "--allow-disconnected", str(brep_path)],
            capture_output=True,
            text=True,
            timeout=300  # Increased for 1024 resolution
        )

        # Parse mesh stats from stdout even if render fails
        stats = parse_mesh_stats(result.stdout)

        if result.returncode != 0:
            print(f"  Raytracer failed: {result.stderr[:200]}", file=sys.stderr)
            print(f"  stdout: {result.stdout[:200]}", file=sys.stderr)
            return False, stats

        # Find the output npy file (raytracer outputs to input file's directory)
        if not npy_file.exists():
            # Try to find it - list all npy files in brep_dir for debugging
            npy_files = list(brep_dir.glob("*.npy"))
            if npy_files:
                # Use the most recently created one
                npy_file = max(npy_files, key=lambda f: f.stat().st_mtime)
            else:
                print(f"  No .npy output found in {brep_dir}", file=sys.stderr)
                print(f"  Expected: {npy_file}", file=sys.stderr)
                print(f"  Raytracer stdout: {result.stdout[:500]}", file=sys.stderr)
                return False, stats

        # Load the data
        data = np.load(npy_file)

        # Data format: [height, width, channels]
        # With --normals --position: 6 channels (X, Y, Z, Nx, Ny, Nz)
        if data.ndim != 3 or data.shape[2] < 6:
            print(f"  Unexpected data shape: {data.shape}", file=sys.stderr)
            return False, stats

        # Extract normals (channels 3, 4, 5)
        normals = data[:, :, 3:6]

        # Create mask for valid hits (non-NaN normals)
        valid_mask = ~np.isnan(normals[:, :, 0])

        # Normalize normals (they should already be normalized, but ensure)
        norm_length = np.sqrt(np.sum(normals**2, axis=2, keepdims=True))
        norm_length[norm_length == 0] = 1  # Avoid division by zero
        normals = normals / norm_length

        # Light direction: top-right-front (normalized)
        light_dir = np.array([0.5, 0.5, 0.7])
        light_dir = light_dir / np.linalg.norm(light_dir)

        # Compute Lambertian shading: dot(N, L)
        # Normals point outward from surface, light points toward surface
        diffuse = np.sum(normals * light_dir, axis=2)
        diffuse = np.clip(diffuse, 0, 1)

        # Add ambient term for shadow areas
        ambient = 0.15
        shading = ambient + (1 - ambient) * diffuse

        # Add subtle rim lighting (based on view direction)
        view_dir = np.array([0, 0, 1])
        rim = 1 - np.abs(np.sum(normals * view_dir, axis=2))
        rim = np.power(rim, 3) * 0.3  # Subtle rim effect

        shading = np.clip(shading + rim, 0, 1)

        # Create RGB image with a pleasant clay color
        base_color = np.array([0.85, 0.75, 0.65])  # Warm clay
        rgb = np.zeros((*shading.shape, 3), dtype=np.float32)

        for i in range(3):
            rgb[:, :, i] = shading * base_color[i]

        # Background color for misses
        bg_color = np.array([0.95, 0.95, 0.95])  # Light gray
        for i in range(3):
            rgb[:, :, i] = np.where(valid_mask, rgb[:, :, i], bg_color[i])

        # Convert to 8-bit
        rgb_8bit = (rgb * 255).astype(np.uint8)

        # Save as PNG
        img = Image.fromarray(rgb_8bit, mode='RGB')
        img.save(output_png)

        # Clean up npy and stl files (they're in the test_data directory)
        npy_file.unlink(missing_ok=True)
        stl_file.unlink(missing_ok=True)

        return True, stats

    except subprocess.TimeoutExpired:
        print(f"  Timeout rendering {brep_file}", file=sys.stderr)
    except Exception as e:
        print(f"  Error rendering {brep_file}: {e}", file=sys.stderr)

    return False, {}


def main():
    parser = argparse.ArgumentParser(
        description="Generate Lambertian-shaded renders from BREP files"
    )
    parser.add_argument("raytracer", help="Path to the raytracer executable")
    parser.add_argument("test_data_dir", help="Directory containing .brep test files")
    parser.add_argument("output_dir", help="Directory to save rendered images")
    parser.add_argument("--resolution", type=int, default=256,
                        help="Image resolution (default: 256)")

    args = parser.parse_args()

    raytracer = args.raytracer
    test_data_dir = Path(args.test_data_dir)
    output_dir = Path(args.output_dir)
    resolution = args.resolution

    # Verify raytracer exists
    if not os.path.isfile(raytracer):
        print(f"Error: Raytracer not found at {raytracer}", file=sys.stderr)
        sys.exit(1)

    # Verify test data directory exists
    if not test_data_dir.is_dir():
        print(f"Error: Test data directory not found at {test_data_dir}", file=sys.stderr)
        sys.exit(1)

    # Create output directory
    output_dir.mkdir(parents=True, exist_ok=True)

    # Find all BREP files
    brep_files = sorted(test_data_dir.glob("*.brep"))
    if not brep_files:
        print(f"Error: No .brep files found in {test_data_dir}", file=sys.stderr)
        sys.exit(1)

    print(f"Found {len(brep_files)} BREP files to render")
    print(f"Resolution: {resolution}x{resolution}")
    print(f"Output: {output_dir}")
    print()

    success_count = 0
    all_stats = {}

    for brep_file in brep_files:
        filename = brep_file.stem
        output_png = output_dir / f"{filename}.png"

        print(f"Rendering: {filename}...")

        success, stats = render_shaded(raytracer, str(brep_file), str(output_png), resolution)
        if success:
            print(f"  -> {output_png}")
            if stats:
                tess_time = stats.get("tessellation_time_ms", "N/A")
                tri_count = stats.get("triangle_count", "N/A")
                face_count = stats.get("cad_face_count", "N/A")
                print(f"     Tessellation: {tess_time} ms, {tri_count} triangles, {face_count} CAD faces")
                if "face_types" in stats:
                    ft = stats["face_types"]
                    types_str = ", ".join(f"{v} {k}" for k, v in ft.items() if v > 0)
                    print(f"     Face types: {types_str}")
            all_stats[filename] = stats
            success_count += 1
        else:
            print(f"  -> FAILED")

    # Write metadata JSON file
    metadata_file = output_dir / "metadata.json"
    with open(metadata_file, "w") as f:
        json.dump(all_stats, f, indent=2)
    print(f"\nMetadata written to: {metadata_file}")

    print()
    print(f"Successfully rendered {success_count}/{len(brep_files)} images")

    if success_count == 0:
        print("Error: No renders completed successfully", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
