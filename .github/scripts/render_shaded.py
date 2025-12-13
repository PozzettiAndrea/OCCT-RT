#!/usr/bin/env python3
"""
Generate renders from OCCT-RT raytracer output for benchmark visualization.

Usage: python render_shaded.py <raytracer> <test_data_dir> <output_dir> [options]

Supports 4 render types matching benchmark levels:
  - raw:       Depth buffer visualization (grayscale)
  - refined:   Same as raw (benchmark measures Newton refinement)
  - normals:   Lambertian shading with clay material
  - curvature: Gaussian curvature map (red=convex, blue=concave)

Also captures mesh statistics (tessellation time, triangle count, face types).
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


# Supported render types
RENDER_TYPES = ["raw", "refined", "normals", "curvature"]


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


def render_depth(data: np.ndarray) -> np.ndarray:
    """
    Render depth buffer visualization (grayscale).

    Args:
        data: Numpy array with position data (channels 0-2 are X, Y, Z)

    Returns:
        RGB numpy array (H, W, 3) as float32 [0-1]
    """
    # Extract Z depth (channel 2)
    depth = data[:, :, 2]

    # Create mask for valid hits
    valid_mask = ~np.isnan(depth)

    # Normalize depth to [0, 1] (invert so closer = brighter)
    valid_depths = depth[valid_mask]
    if len(valid_depths) == 0:
        # No valid hits
        return np.ones((*depth.shape, 3), dtype=np.float32) * 0.95

    depth_min = np.min(valid_depths)
    depth_max = np.max(valid_depths)
    depth_range = depth_max - depth_min

    if depth_range < 1e-6:
        normalized = np.ones_like(depth) * 0.5
    else:
        # Closer objects (smaller Z) should be brighter
        normalized = 1.0 - (depth - depth_min) / depth_range

    # Apply gamma for better visual contrast
    normalized = np.power(np.clip(normalized, 0, 1), 0.7)

    # Create grayscale RGB
    rgb = np.zeros((*depth.shape, 3), dtype=np.float32)
    for i in range(3):
        rgb[:, :, i] = normalized

    # Background color for misses
    bg_color = 0.95
    for i in range(3):
        rgb[:, :, i] = np.where(valid_mask, rgb[:, :, i], bg_color)

    return rgb


def render_normals(data: np.ndarray) -> np.ndarray:
    """
    Render Lambertian shading with clay material.

    Args:
        data: Numpy array with position (0-2) and normal (3-5) data

    Returns:
        RGB numpy array (H, W, 3) as float32 [0-1]
    """
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

    return rgb


def render_curvature(data: np.ndarray, curv_channel: int) -> np.ndarray:
    """
    Render Gaussian curvature visualization (diverging colormap).

    Args:
        data: Numpy array with curvature data at specified channel
        curv_channel: Channel index for Gaussian curvature

    Returns:
        RGB numpy array (H, W, 3) as float32 [0-1]
    """
    # Extract Gaussian curvature
    curvature = data[:, :, curv_channel]

    # Create mask for valid hits
    valid_mask = ~np.isnan(curvature)

    # Get valid curvature values for normalization
    valid_curv = curvature[valid_mask]
    if len(valid_curv) == 0:
        return np.ones((*curvature.shape, 3), dtype=np.float32) * 0.95

    # Use symmetric normalization around 0
    # Positive curvature = convex (red), Negative = concave (blue)
    abs_max = np.percentile(np.abs(valid_curv), 98)  # Use 98th percentile to avoid outliers
    if abs_max < 1e-10:
        abs_max = 1.0

    # Normalize to [-1, 1]
    normalized = np.clip(curvature / abs_max, -1, 1)

    # Create diverging colormap: blue (-1) -> white (0) -> red (+1)
    rgb = np.zeros((*curvature.shape, 3), dtype=np.float32)

    # Red channel: increases with positive curvature
    rgb[:, :, 0] = np.clip(0.5 + 0.5 * normalized, 0, 1)

    # Green channel: peaks at zero
    rgb[:, :, 1] = 1.0 - np.abs(normalized) * 0.5

    # Blue channel: increases with negative curvature
    rgb[:, :, 2] = np.clip(0.5 - 0.5 * normalized, 0, 1)

    # Background color for misses
    bg_color = 0.95
    for i in range(3):
        rgb[:, :, i] = np.where(valid_mask, rgb[:, :, i], bg_color)

    return rgb


def render_geometry(raytracer: str, brep_file: str, output_png: str,
                    render_type: str, resolution: int = 256) -> tuple:
    """
    Render a BREP file with specified visualization type.

    Args:
        raytracer: Path to raytracer executable
        brep_file: Path to BREP file
        output_png: Path to output PNG file
        render_type: One of "raw", "refined", "normals", "curvature"
        resolution: Image resolution

    Returns:
        Tuple of (success: bool, stats: dict)
    """
    brep_path = Path(brep_file).resolve()
    brep_dir = brep_path.parent
    brep_stem = brep_path.stem
    npy_file = brep_dir / f"{brep_stem}_data.npy"
    stl_file = brep_dir / f"{brep_stem}_tessellation.stl"

    # Build raytracer command based on render type
    cmd = [raytracer, "-r", str(resolution), "--position", "-s", "--allow-disconnected"]

    if render_type in ("raw", "refined"):
        # Just position data for depth visualization
        pass
    elif render_type == "normals":
        cmd.append("--normals")
    elif render_type == "curvature":
        cmd.extend(["--normals", "--curvature-gauss"])

    cmd.append(str(brep_path))

    try:
        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=300
        )

        # Parse mesh stats from stdout
        stats = parse_mesh_stats(result.stdout)

        if result.returncode != 0:
            print(f"  Raytracer failed: {result.stderr[:200]}", file=sys.stderr)
            return False, stats

        # Find the output npy file
        if not npy_file.exists():
            npy_files = list(brep_dir.glob("*.npy"))
            if npy_files:
                npy_file = max(npy_files, key=lambda f: f.stat().st_mtime)
            else:
                print(f"  No .npy output found in {brep_dir}", file=sys.stderr)
                return False, stats

        # Load the data
        data = np.load(npy_file)

        # Generate visualization based on type
        if render_type in ("raw", "refined"):
            # Depth buffer (grayscale)
            if data.ndim != 3 or data.shape[2] < 3:
                print(f"  Unexpected data shape for depth: {data.shape}", file=sys.stderr)
                return False, stats
            rgb = render_depth(data)
        elif render_type == "normals":
            # Lambertian shading
            if data.ndim != 3 or data.shape[2] < 6:
                print(f"  Unexpected data shape for normals: {data.shape}", file=sys.stderr)
                return False, stats
            rgb = render_normals(data)
        elif render_type == "curvature":
            # Curvature visualization
            # With --position --normals --curvature-gauss: 7 channels (X,Y,Z,Nx,Ny,Nz,K)
            if data.ndim != 3 or data.shape[2] < 7:
                print(f"  Unexpected data shape for curvature: {data.shape}", file=sys.stderr)
                return False, stats
            rgb = render_curvature(data, curv_channel=6)
        else:
            print(f"  Unknown render type: {render_type}", file=sys.stderr)
            return False, stats

        # Convert to 8-bit and save
        rgb_8bit = (rgb * 255).astype(np.uint8)
        img = Image.fromarray(rgb_8bit, mode='RGB')
        img.save(output_png)

        # Clean up
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
        description="Generate renders from BREP files for benchmark visualization"
    )
    parser.add_argument("raytracer", help="Path to the raytracer executable")
    parser.add_argument("test_data_dir", help="Directory containing .brep test files")
    parser.add_argument("output_dir", help="Directory to save rendered images")
    parser.add_argument("--resolution", type=int, default=256,
                        help="Image resolution (default: 256)")
    parser.add_argument("--render-types", nargs="+", default=["normals"],
                        choices=RENDER_TYPES + ["all"],
                        help="Render types to generate (default: normals). "
                             "Use 'all' for all types.")

    args = parser.parse_args()

    raytracer = args.raytracer
    test_data_dir = Path(args.test_data_dir)
    output_dir = Path(args.output_dir)
    resolution = args.resolution

    # Expand "all" to all render types
    render_types = args.render_types
    if "all" in render_types:
        render_types = RENDER_TYPES

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
    print(f"Render types: {', '.join(render_types)}")
    print(f"Output: {output_dir}")
    print()

    success_count = 0
    total_renders = len(brep_files) * len(render_types)
    all_stats = {}

    for brep_file in brep_files:
        filename = brep_file.stem
        all_stats[filename] = {}

        for render_type in render_types:
            # Output filename includes render type suffix
            output_png = output_dir / f"{filename}_{render_type}.png"

            print(f"Rendering: {filename} ({render_type})...")

            success, stats = render_geometry(raytracer, str(brep_file),
                                             str(output_png), render_type, resolution)
            if success:
                print(f"  -> {output_png.name}")
                success_count += 1

                # Store stats (same for all render types of same geometry)
                if stats and not all_stats[filename]:
                    all_stats[filename] = stats
                    tess_time = stats.get("tessellation_time_ms", "N/A")
                    tri_count = stats.get("triangle_count", "N/A")
                    face_count = stats.get("cad_face_count", "N/A")
                    print(f"     {tri_count} triangles, {face_count} CAD faces, {tess_time}ms tessellation")
            else:
                print(f"  -> FAILED")

    # Write metadata JSON file
    metadata_file = output_dir / "metadata.json"
    with open(metadata_file, "w") as f:
        json.dump(all_stats, f, indent=2)
    print(f"\nMetadata written to: {metadata_file}")

    print()
    print(f"Successfully rendered {success_count}/{total_renders} images")

    if success_count == 0:
        print("Error: No renders completed successfully", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
