// OCCT-RT Raytracer Command-Line Tool
// High-performance BVH-based raytracer for OpenCASCADE BREP files
//
// Usage: raytracer [options] <brep_file_path>

#include <BRepIntCurveSurface_InterBVH.hxx>
#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <gp_Lin.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <OSD_Timer.hxx>
#include <NCollection_Array1.hxx>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdint>
#include <set>
#include <functional>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

//=============================================================================
// Simple BMP writer (no external dependencies)
//=============================================================================

#pragma pack(push, 1)

struct BMPHeader
{
  uint16_t type; // "BM"
  uint32_t size; // File size
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offset;     // Offset to pixel data
  uint32_t headerSize; // DIB header size (40)
  int32_t  width;
  int32_t  height;
  uint16_t planes;      // 1
  uint16_t bpp;         // 24
  uint32_t compression; // 0
  uint32_t imageSize;
  int32_t  xPPM;
  int32_t  yPPM;
  uint32_t colorsUsed;
  uint32_t colorsImportant;
};

#pragma pack(pop)

bool WriteBMP(const std::string&          filename,
              const std::vector<uint8_t>& pixels,
              int                         width,
              int                         height)
{
  int rowSize   = (width * 3 + 3) & ~3; // Rows padded to 4-byte boundary
  int imageSize = rowSize * height;

  BMPHeader header   = {};
  header.type        = 0x4D42; // "BM"
  header.size        = sizeof(BMPHeader) + imageSize;
  header.offset      = sizeof(BMPHeader);
  header.headerSize  = 40;
  header.width       = width;
  header.height      = height;
  header.planes      = 1;
  header.bpp         = 24;
  header.compression = 0;
  header.imageSize   = imageSize;

  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    std::cerr << "Error: Cannot create file " << filename << std::endl;
    return false;
  }

  file.write(reinterpret_cast<const char*>(&header), sizeof(header));

  // Write pixels (BMP is bottom-up, BGR format)
  std::vector<uint8_t> row(rowSize, 0);
  for (int y = height - 1; y >= 0; --y)
  {
    for (int x = 0; x < width; ++x)
    {
      int srcIdx     = (y * width + x) * 3;
      row[x * 3 + 0] = pixels[srcIdx + 2]; // B
      row[x * 3 + 1] = pixels[srcIdx + 1]; // G
      row[x * 3 + 2] = pixels[srcIdx + 0]; // R
    }
    file.write(reinterpret_cast<const char*>(row.data()), rowSize);
  }

  return true;
}

//=============================================================================
// Simple NumPy .npy writer (no external dependencies)
//=============================================================================

bool WriteNPY(const std::string&        filename,
              const std::vector<float>& data,
              int                       height,
              int                       width,
              int                       channels)
{
  std::ofstream file(filename, std::ios::binary);
  if (!file)
  {
    std::cerr << "Error: Cannot create file " << filename << std::endl;
    return false;
  }

  // Build header string
  std::string header = "{'descr': '<f4', 'fortran_order': False, 'shape': (";
  header += std::to_string(height) + ", " + std::to_string(width);
  if (channels > 1)
  {
    header += ", " + std::to_string(channels);
  }
  header += "), }";

  // Pad header to align total header to 64 bytes
  // Total = 6 (magic) + 2 (version) + 2 (header_len) + header + 1 (newline)
  size_t totalLen = 10 + header.size() + 1;
  size_t padding  = (64 - (totalLen % 64)) % 64;
  header += std::string(padding, ' ');
  header += '\n';

  uint16_t headerLen = static_cast<uint16_t>(header.size());

  // Write magic string and version
  const char magic[] = "\x93NUMPY";
  file.write(magic, 6);

  // Version 1.0
  char version[2] = {1, 0};
  file.write(version, 2);

  // Header length (little-endian)
  file.write(reinterpret_cast<const char*>(&headerLen), 2);

  // Header
  file.write(header.c_str(), header.size());

  // Data (float32, C-order)
  file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(float));

  return true;
}

//=============================================================================
// Simple STL writer (no external dependencies)
//=============================================================================

bool WriteSTL(const std::string& filename, const TopoDS_Shape& shape)
{
  std::ofstream file(filename);
  if (!file)
  {
    std::cerr << "Error: Cannot create file " << filename << std::endl;
    return false;
  }

  file << "solid tessellation\n";
  file << std::fixed << std::setprecision(6);

  int totalTriangles = 0;

  for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next())
  {
    const TopoDS_Face&         face = TopoDS::Face(exp.Current());
    TopLoc_Location            loc;
    Handle(Poly_Triangulation) tri = BRep_Tool::Triangulation(face, loc);

    if (tri.IsNull())
      continue;

    const gp_Trsf& trsf         = loc.Transformation();
    bool           hasTransform = (loc.IsIdentity() == Standard_False);

    for (int i = 1; i <= tri->NbTriangles(); ++i)
    {
      const Poly_Triangle& triangle = tri->Triangle(i);
      int                  n1, n2, n3;
      triangle.Get(n1, n2, n3);

      gp_Pnt p1 = tri->Node(n1);
      gp_Pnt p2 = tri->Node(n2);
      gp_Pnt p3 = tri->Node(n3);

      if (hasTransform)
      {
        p1.Transform(trsf);
        p2.Transform(trsf);
        p3.Transform(trsf);
      }

      // Handle face orientation
      if (face.Orientation() == TopAbs_REVERSED)
        std::swap(p2, p3);

      // Compute normal
      gp_Vec v1(p1, p2);
      gp_Vec v2(p1, p3);
      gp_Vec normal = v1.Crossed(v2);
      if (normal.Magnitude() > 1e-10)
        normal.Normalize();
      else
        normal = gp_Vec(0, 0, 1);

      file << "  facet normal " << normal.X() << " " << normal.Y() << " " << normal.Z() << "\n";
      file << "    outer loop\n";
      file << "      vertex " << p1.X() << " " << p1.Y() << " " << p1.Z() << "\n";
      file << "      vertex " << p2.X() << " " << p2.Y() << " " << p2.Z() << "\n";
      file << "      vertex " << p3.X() << " " << p3.Y() << " " << p3.Z() << "\n";
      file << "    endloop\n";
      file << "  endfacet\n";

      totalTriangles++;
    }
  }

  file << "endsolid tessellation\n";

  std::cout << "Wrote " << totalTriangles << " triangles to STL" << std::endl;
  return true;
}

//=============================================================================
// Connected component analysis
//=============================================================================

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>

//! Count number of face-connected components in a shape
//! Returns 1 if all faces are connected through shared edges
int CountConnectedComponents(const TopoDS_Shape& theShape)
{
  // Get all faces
  TopTools_IndexedMapOfShape faceMap;
  TopExp::MapShapes(theShape, TopAbs_FACE, faceMap);

  int nbFaces = faceMap.Extent();
  if (nbFaces == 0)
    return 0;
  if (nbFaces == 1)
    return 1;

  // Build edge-to-face adjacency map
  TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;
  TopExp::MapShapesAndAncestors(theShape, TopAbs_EDGE, TopAbs_FACE, edgeFaceMap);

  // Union-Find data structure
  std::vector<int> parent(nbFaces + 1);
  std::vector<int> rank(nbFaces + 1, 0);
  for (int i = 0; i <= nbFaces; ++i)
    parent[i] = i;

  // Find with path compression
  std::function<int(int)> find = [&](int x) -> int {
    if (parent[x] != x)
      parent[x] = find(parent[x]);
    return parent[x];
  };

  // Union by rank
  auto unite = [&](int x, int y) {
    int px = find(x), py = find(y);
    if (px == py)
      return;
    if (rank[px] < rank[py])
      std::swap(px, py);
    parent[py] = px;
    if (rank[px] == rank[py])
      rank[px]++;
  };

  // Unite faces that share edges
  for (int e = 1; e <= edgeFaceMap.Extent(); ++e)
  {
    const TopTools_ListOfShape& faces = edgeFaceMap(e);
    if (faces.Extent() < 2)
      continue;

    // Get first face index
    int firstFaceIdx = faceMap.FindIndex(faces.First());

    // Unite with all other faces sharing this edge
    for (TopTools_ListIteratorOfListOfShape it(faces); it.More(); it.Next())
    {
      int faceIdx = faceMap.FindIndex(it.Value());
      unite(firstFaceIdx, faceIdx);
    }
  }

  // Count unique roots
  std::set<int> roots;
  for (int i = 1; i <= nbFaces; ++i)
    roots.insert(find(i));

  return static_cast<int>(roots.size());
}

//=============================================================================
// CAD loading and raytracing
//=============================================================================

bool LoadBREP(const std::string& thePath, TopoDS_Shape& theShape)
{
  BRep_Builder builder;
  if (!BRepTools::Read(theShape, thePath.c_str(), builder))
  {
    std::cerr << "Error: Failed to read BREP file: " << thePath << std::endl;
    return false;
  }
  return true;
}

//! Generate rays with correct aspect ratio
void GenerateImageRays(NCollection_Array1<gp_Lin>& theRays,
                       int&                        outWidth,
                       int&                        outHeight,
                       int                         theMaxDim,
                       const Bnd_Box&              theBndBox,
                       Standard_Real               theMargin = 1.1)
{
  Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
  theBndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  Standard_Real cx      = (xmin + xmax) / 2.0;
  Standard_Real cy      = (ymin + ymax) / 2.0;
  Standard_Real extentX = (xmax - xmin) * theMargin;
  Standard_Real extentY = (ymax - ymin) * theMargin;
  Standard_Real zHeight = zmax + (zmax - zmin) * 0.5;

  // Calculate image dimensions preserving aspect ratio
  if (extentX >= extentY)
  {
    outWidth  = theMaxDim;
    outHeight = (int)(theMaxDim * extentY / extentX);
  }
  else
  {
    outHeight = theMaxDim;
    outWidth  = (int)(theMaxDim * extentX / extentY);
  }
  if (outWidth < 1)
    outWidth = 1;
  if (outHeight < 1)
    outHeight = 1;

  Standard_Integer nRays = outWidth * outHeight;
  theRays.Resize(1, nRays, Standard_False);

  Standard_Real stepX = extentX / (outWidth - 1);
  Standard_Real stepY = extentY / (outHeight - 1);

  Standard_Integer idx = 1;
  for (int iy = 0; iy < outHeight; ++iy)
  {
    for (int ix = 0; ix < outWidth; ++ix)
    {
      Standard_Real x = cx - extentX / 2.0 + ix * stepX;
      Standard_Real y = cy - extentY / 2.0 + iy * stepY;

      gp_Pnt origin(x, y, zHeight);
      gp_Dir direction(0, 0, -1);

      theRays(idx++) = gp_Lin(origin, direction);
    }
  }
}

//! Render Z-height image and save to BMP
//! @param theWithNormals If true, encode normals in G and B channels (R=Z, G=Nx, B=Ny)
void RenderZHeightImage(BRepIntCurveSurface_InterBVH& theRaytracer,
                        const Bnd_Box&                theBndBox,
                        int                           theMaxDim,
                        const std::string&            theOutputPath,
                        bool                          theWithNormals = false)
{
  int                        width, height;
  NCollection_Array1<gp_Lin> rays;
  GenerateImageRays(rays, width, height, theMaxDim, theBndBox);

  std::cout << "Rendering Z-height " << width << "x" << height << " image (" << rays.Length()
            << " rays)..." << std::endl;

  NCollection_Array1<BRepIntCurveSurface_HitResult> results;

  OSD_Timer timer;
  timer.Start();
  theRaytracer.PerformBatch(rays, results);
  timer.Stop();

  // Find Z range for normalization
  Standard_Real xmin_t, ymin_t, zmin_t, xmax_t, ymax_t, zmax_t;
  theBndBox.Get(xmin_t, ymin_t, zmin_t, xmax_t, ymax_t, zmax_t);
  Standard_Real zmin   = zmin_t;
  Standard_Real zmax   = zmax_t;
  Standard_Real zRange = zmax - zmin;
  if (zRange < 1e-10)
    zRange = 1.0;

  // Create pixel buffer (RGB) - black background
  std::vector<uint8_t> pixels(width * height * 3, 0);

  int hitCount = 0;
  for (int iy = 0; iy < height; ++iy)
  {
    for (int ix = 0; ix < width; ++ix)
    {
      int                                  rayIdx = iy * width + ix + 1; // 1-based
      const BRepIntCurveSurface_HitResult& hit    = results(rayIdx);

      // Flip Y so higher Y values appear at top of image (consistent orientation)
      int pixIdx = ((height - 1 - iy) * width + ix) * 3;

      if (hit.IsValid)
      {
        hitCount++;
        // Map Z height to grayscale 0-255 (higher = brighter)
        Standard_Real z = hit.Point.Z();
        Standard_Real t = (z - zmin) / zRange; // 0 to 1
        t               = std::max(0.0, std::min(1.0, t));
        uint8_t gray    = (uint8_t)(t * 255);

        if (theWithNormals)
        {
          // R = Z height, G = normal X, B = normal Y
          // Map normal components from [-1, 1] to [0, 255]
          uint8_t normalX = (uint8_t)((hit.Normal.X() + 1.0) * 0.5 * 255.0);
          uint8_t normalY = (uint8_t)((hit.Normal.Y() + 1.0) * 0.5 * 255.0);

          pixels[pixIdx + 0] = gray;    // R = Z height
          pixels[pixIdx + 1] = normalX; // G = normal X
          pixels[pixIdx + 2] = normalY; // B = normal Y
        }
        else
        {
          pixels[pixIdx + 0] = gray; // R
          pixels[pixIdx + 1] = gray; // G
          pixels[pixIdx + 2] = gray; // B
        }
      }
    }
  }

  if (WriteBMP(theOutputPath, pixels, width, height))
    std::cout << "Saved: " << theOutputPath << std::endl;

  Standard_Real elapsedMs  = timer.ElapsedTime() * 1000.0;
  Standard_Real raysPerSec = (elapsedMs > 0) ? (rays.Length() / (elapsedMs / 1000.0)) : 0;

  std::cout << "  " << hitCount << " hits, " << std::fixed << std::setprecision(0) << elapsedMs
            << " ms, " << raysPerSec << " rays/sec" << std::endl;
}

//! Render intersection count image (green=1 to red=5+)
void RenderHitCountImage(BRepIntCurveSurface_InterBVH& theRaytracer,
                         const Bnd_Box&                theBndBox,
                         int                           theMaxDim,
                         const std::string&            theOutDir,
                         const std::string&            theBaseName)
{
  int                        width, height;
  NCollection_Array1<gp_Lin> rays;
  GenerateImageRays(rays, width, height, theMaxDim, theBndBox);

  std::cout << "Rendering hit-count " << width << "x" << height << " image (" << rays.Length()
            << " rays)..." << std::endl;

  NCollection_Array1<Standard_Integer> hitCounts;

  OSD_Timer timer;
  timer.Start();
  theRaytracer.PerformBatchCount(rays, hitCounts);
  timer.Stop();

  // Create pixel buffer (RGB) - black background
  std::vector<uint8_t> pixels(width * height * 3, 0);

  // Color map: 0=black, 1=green, 2=yellow, 3=orange, 4=red-orange, 5+=red
  auto getColor = [](int count, uint8_t& r, uint8_t& g, uint8_t& b) {
    if (count == 0)
    {
      r = g = b = 0; // Black - miss
    }
    else if (count == 1)
    {
      r = 0;
      g = 255;
      b = 0; // Green - 1 hit
    }
    else if (count == 2)
    {
      r = 255;
      g = 255;
      b = 0; // Yellow - 2 hits
    }
    else if (count == 3)
    {
      r = 255;
      g = 165;
      b = 0; // Orange - 3 hits
    }
    else if (count == 4)
    {
      r = 255;
      g = 69;
      b = 0; // Red-Orange - 4 hits
    }
    else
    {
      r = 255;
      g = 0;
      b = 0; // Red - 5+ hits
    }
  };

  int totalHits = 0;
  int maxHits   = 0;
  for (int iy = 0; iy < height; ++iy)
  {
    for (int ix = 0; ix < width; ++ix)
    {
      int rayIdx = iy * width + ix + 1;
      int count  = hitCounts(rayIdx);

      // Flip Y so higher Y values appear at top of image (consistent orientation)
      int pixIdx = ((height - 1 - iy) * width + ix) * 3;

      uint8_t r, g, b;
      getColor(count, r, g, b);
      pixels[pixIdx + 0] = r;
      pixels[pixIdx + 1] = g;
      pixels[pixIdx + 2] = b;

      totalHits += count;
      if (count > maxHits)
        maxHits = count;
    }
  }

  std::string outPath =
    theOutDir + "/" + theBaseName + "_hitcount_max" + std::to_string(maxHits) + ".bmp";
  if (WriteBMP(outPath, pixels, width, height))
    std::cout << "Saved: " << outPath << std::endl;

  Standard_Real elapsedMs  = timer.ElapsedTime() * 1000.0;
  Standard_Real raysPerSec = (elapsedMs > 0) ? (rays.Length() / (elapsedMs / 1000.0)) : 0;

  std::cout << "  Total intersections: " << totalHits << ", max per ray: " << maxHits << std::endl;
  std::cout << "  " << std::fixed << std::setprecision(0) << elapsedMs << " ms, " << raysPerSec
            << " rays/sec" << std::endl;
}

//! Render selected channels to NumPy .npy file
void RenderToNumpy(BRepIntCurveSurface_InterBVH& theRaytracer,
                   const Bnd_Box&                theBndBox,
                   int                           theMaxDim,
                   const std::string&            theOutputPath,
                   bool                          outPosition,
                   bool                          outHeight,
                   bool                          outNormals,
                   bool                          outFaceId,
                   bool                          outCurvGauss,
                   bool                          outCurvMean,
                   bool                          outCurvMin,
                   bool                          outCurvMax)
{
  // Count output channels
  int numChannels = 0;
  if (outPosition)
    numChannels += 3; // X, Y, Z
  if (outHeight)
    numChannels += 1; // Z only
  if (outNormals)
    numChannels += 3; // Nx, Ny, Nz
  if (outFaceId)
    numChannels += 1; // Face ID
  if (outCurvGauss)
    numChannels += 1; // Gaussian K
  if (outCurvMean)
    numChannels += 1; // Mean H
  if (outCurvMin)
    numChannels += 1; // Min principal
  if (outCurvMax)
    numChannels += 1; // Max principal

  if (numChannels == 0)
  {
    std::cerr << "Warning: No output channels selected for NumPy output" << std::endl;
    return;
  }

  int                        width, height;
  NCollection_Array1<gp_Lin> rays;
  GenerateImageRays(rays, width, height, theMaxDim, theBndBox);

  std::cout << "Rendering " << width << "x" << height << " (" << numChannels
            << " channels) to NumPy..." << std::endl;

  // Build channel description for user
  std::cout << "  Channels: ";
  std::vector<std::string> channelNames;
  if (outPosition)
  {
    channelNames.push_back("PosX");
    channelNames.push_back("PosY");
    channelNames.push_back("PosZ");
  }
  if (outHeight)
  {
    channelNames.push_back("Height");
  }
  if (outNormals)
  {
    channelNames.push_back("NormX");
    channelNames.push_back("NormY");
    channelNames.push_back("NormZ");
  }
  if (outFaceId)
  {
    channelNames.push_back("FaceID");
  }
  if (outCurvGauss)
  {
    channelNames.push_back("GaussK");
  }
  if (outCurvMean)
  {
    channelNames.push_back("MeanH");
  }
  if (outCurvMin)
  {
    channelNames.push_back("CurvMin");
  }
  if (outCurvMax)
  {
    channelNames.push_back("CurvMax");
  }
  for (size_t i = 0; i < channelNames.size(); ++i)
  {
    if (i > 0)
      std::cout << ", ";
    std::cout << channelNames[i];
  }
  std::cout << std::endl;

  NCollection_Array1<BRepIntCurveSurface_HitResult> results;

  OSD_Timer timer;
  timer.Start();
  theRaytracer.PerformBatch(rays, results);
  timer.Stop();

  // Create output buffer (HxWxC layout for NumPy)
  std::vector<float> data(static_cast<size_t>(height) * width * numChannels, 0.0f);

  int hitCount = 0;
  for (int iy = 0; iy < height; ++iy)
  {
    for (int ix = 0; ix < width; ++ix)
    {
      int                                  rayIdx = iy * width + ix + 1; // 1-based
      const BRepIntCurveSurface_HitResult& hit    = results(rayIdx);

      // Flip Y so higher Y values appear at top of image
      int    outY    = height - 1 - iy;
      size_t baseIdx = (static_cast<size_t>(outY) * width + ix) * numChannels;

      if (hit.IsValid)
      {
        hitCount++;
        int ch = 0;

        if (outPosition)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.Point.X());
          data[baseIdx + ch++] = static_cast<float>(hit.Point.Y());
          data[baseIdx + ch++] = static_cast<float>(hit.Point.Z());
        }
        if (outHeight)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.Point.Z());
        }
        if (outNormals)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.Normal.X());
          data[baseIdx + ch++] = static_cast<float>(hit.Normal.Y());
          data[baseIdx + ch++] = static_cast<float>(hit.Normal.Z());
        }
        if (outFaceId)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.FaceIndex);
        }
        if (outCurvGauss)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.GaussianCurvature);
        }
        if (outCurvMean)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.MeanCurvature);
        }
        if (outCurvMin)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.MinCurvature);
        }
        if (outCurvMax)
        {
          data[baseIdx + ch++] = static_cast<float>(hit.MaxCurvature);
        }
      }
      else
      {
        // Miss - fill with NaN for all channels
        for (int ch = 0; ch < numChannels; ++ch)
        {
          data[baseIdx + ch] = std::nanf("");
        }
      }
    }
  }

  if (WriteNPY(theOutputPath, data, height, width, numChannels))
    std::cout << "Saved: " << theOutputPath << std::endl;

  Standard_Real elapsedMs  = timer.ElapsedTime() * 1000.0;
  Standard_Real raysPerSec = (elapsedMs > 0) ? (rays.Length() / (elapsedMs / 1000.0)) : 0;

  std::cout << "  " << hitCount << " hits, " << std::fixed << std::setprecision(0) << elapsedMs
            << " ms, " << raysPerSec << " rays/sec" << std::endl;
}

//! Get directory from file path
std::string GetDirectory(const std::string& path)
{
  size_t pos = path.find_last_of("/\\");
  if (pos == std::string::npos)
    return ".";
  return path.substr(0, pos);
}

//! Get filename without extension
std::string GetBasename(const std::string& path)
{
  size_t pos1  = path.find_last_of("/\\");
  size_t start = (pos1 == std::string::npos) ? 0 : pos1 + 1;
  size_t pos2  = path.rfind('.');
  if (pos2 == std::string::npos || pos2 < start)
    return path.substr(start);
  return path.substr(start, pos2 - start);
}

//=============================================================================
// Benchmark functions
//=============================================================================

void GenerateGridRays(NCollection_Array1<gp_Lin>& theRays,
                      Standard_Integer            theResolution,
                      const Bnd_Box&              theBndBox,
                      Standard_Real               theMargin = 1.1)
{
  Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
  theBndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  Standard_Real cx      = (xmin + xmax) / 2.0;
  Standard_Real cy      = (ymin + ymax) / 2.0;
  Standard_Real extentX = (xmax - xmin) * theMargin;
  Standard_Real extentY = (ymax - ymin) * theMargin;
  Standard_Real zHeight = zmax + (zmax - zmin) * 0.5;

  Standard_Integer nRays = theResolution * theResolution;
  theRays.Resize(1, nRays, Standard_False);

  Standard_Real stepX = extentX / (theResolution - 1);
  Standard_Real stepY = extentY / (theResolution - 1);

  Standard_Integer idx = 1;
  for (Standard_Integer iy = 0; iy < theResolution; ++iy)
  {
    for (Standard_Integer ix = 0; ix < theResolution; ++ix)
    {
      Standard_Real x = cx - extentX / 2.0 + ix * stepX;
      Standard_Real y = cy - extentY / 2.0 + iy * stepY;

      gp_Pnt origin(x, y, zHeight);
      gp_Dir direction(0, 0, -1);

      theRays(idx++) = gp_Lin(origin, direction);
    }
  }
}

void RunBenchmark(BRepIntCurveSurface_InterBVH& theRaytracer,
                  Standard_Integer              theResolution,
                  const Bnd_Box&                theBndBox,
                  bool                          useCoherent = false)
{
  Standard_Integer nRays = theResolution * theResolution;

  NCollection_Array1<gp_Lin> rays;
  GenerateGridRays(rays, theResolution, theBndBox);

  NCollection_Array1<BRepIntCurveSurface_HitResult> results;

  OSD_Timer timer;
  timer.Start();
  if (useCoherent)
  {
    theRaytracer.PerformBatchCoherent(rays, results);
  }
  else
  {
    theRaytracer.PerformBatch(rays, results);
  }
  timer.Stop();

  Standard_Integer hitCount = 0;
  for (Standard_Integer i = results.Lower(); i <= results.Upper(); ++i)
  {
    if (results(i).IsValid)
      hitCount++;
  }

  Standard_Real elapsedMs  = timer.ElapsedTime() * 1000.0;
  Standard_Real raysPerSec = (elapsedMs > 0) ? (nRays / (elapsedMs / 1000.0)) : 0;

  std::cout << std::setw(10) << nRays << " rays | " << std::setw(8) << hitCount << " hits | "
            << std::fixed << std::setprecision(2) << std::setw(10) << elapsedMs << " ms | "
            << std::setprecision(0) << std::setw(10) << raysPerSec << " rays/sec" << std::endl;
}

//=============================================================================
// Main
//=============================================================================

void PrintUsage(const char* progName)
{
  std::cout << "Usage: " << progName << " [options] <brep_file_path>" << std::endl;
  std::cout << std::endl;
  std::cout << "Performance Options:" << std::endl;
  std::cout << "  --backend BACKEND   BVH backend: occt, embree, embree4, embree8 (default: occt)"
            << std::endl;
  std::cout << "                      occt    = OCCT built-in BVH (best single-ray)" << std::endl;
  std::cout << "                      embree  = Embree rtcIntersect1 (scalar)" << std::endl;
  std::cout << "                      embree4 = Embree rtcIntersect4 (SSE, 4 rays)" << std::endl;
  std::cout << "                      embree8 = Embree rtcIntersect8 (AVX, 8 rays)" << std::endl;
  std::cout << "  --openmp            Enable OpenMP parallelization (default: on)" << std::endl;
  std::cout << "  --no-openmp         Disable OpenMP parallelization" << std::endl;
  std::cout << "  --coherent          Use coherent packet raytracing (groups rays by direction)"
            << std::endl;
  std::cout << "  --no-sorting        Disable ray sorting for coherent tracing" << std::endl;
  std::cout << std::endl;
  std::cout << "NumPy Output Options (mix and match, outputs float32 .npy file):" << std::endl;
  std::cout << "  --position          Output hit point X/Y/Z coordinates (3 channels)" << std::endl;
  std::cout << "  --height            Output hit point Z coordinate only (1 channel)" << std::endl;
  std::cout << "  --normals           Output surface normal X/Y/Z (3 channels)" << std::endl;
  std::cout << "  --faceid            Output face ID (1 channel)" << std::endl;
  std::cout << "  --curvature-gauss   Output Gaussian curvature K (1 channel)" << std::endl;
  std::cout << "  --curvature-mean    Output Mean curvature H (1 channel)" << std::endl;
  std::cout << "  --curvature-min     Output min principal curvature (1 channel)" << std::endl;
  std::cout << "  --curvature-max     Output max principal curvature (1 channel)" << std::endl;
  std::cout << "  --curvatures        Output all 4 curvature types (4 channels)" << std::endl;
  std::cout << "  --all               Output all available channels (13 total)" << std::endl;
  std::cout << std::endl;
  std::cout << "BMP Output Options:" << std::endl;
  std::cout << "  -o, --output-image  Output Z-height image (BMP, 8-bit)" << std::endl;
  std::cout << "  -N, --with-normals  Include normals in Z-height BMP (R=Z, G=Nx, B=Ny)"
            << std::endl;
  std::cout << "  -n, --hit-count     Output hit count image (green=1 to red=5+)" << std::endl;
  std::cout << std::endl;
  std::cout << "General Options:" << std::endl;
  std::cout << "  -h, --help          Show this help message" << std::endl;
  std::cout << "  -r, --resolution N  Image resolution (max dimension, default 500)" << std::endl;
  std::cout << "  -b, --benchmark     Run batch raytracing benchmarks" << std::endl;
  std::cout << "  -d, --deflection D  Tessellation deflection (default 0.02)" << std::endl;
  std::cout << "                      Smaller = finer mesh, more accurate" << std::endl;
  std::cout << "  -a, --angle A       Angular deflection in radians (default 0.1)" << std::endl;
  std::cout << "                      Controls segment count on curves" << std::endl;
  std::cout << "  --newton-tol T      Newton refinement tolerance (default 1e-7)" << std::endl;
  std::cout << "                      Larger values (1e-4) = faster but less precise" << std::endl;
  std::cout << "  --newton-iter N     Max Newton iterations (default 10)" << std::endl;
  std::cout << "                      Lower values (3-5) = faster convergence" << std::endl;
  std::cout << "  --simd-newton       Enable SIMD-accelerated Newton batching (experimental)" << std::endl;
  std::cout << "  -s, --export-stl    Export tessellation as STL file" << std::endl;
  std::cout << "  --roi X1,Y1,X2,Y2   Region of interest (XY bounds for raytracing)" << std::endl;
  std::cout << std::endl;
  std::cout << "Output file naming:" << std::endl;
  std::cout << "  NumPy:      {input}_data.npy" << std::endl;
  std::cout << "  Z-height:   {input}_zheight.bmp (or {input}_zheight_normals.bmp with -N)"
            << std::endl;
  std::cout << "  Hit count:  {input}_hitcount_max{N}.bmp (N = max hits per ray)" << std::endl;
}

int main(int argc, char* argv[])
{
  std::cout << "========================================" << std::endl;
  std::cout << "  OCCT-RT Raytracer" << std::endl;
  std::cout << "========================================" << std::endl;

  std::string inputFile;
  bool        outputZHeight     = false;
  bool        outputWithNormals = false;
  bool        outputHitCount    = false;
  bool        runBenchmarks     = false;
  bool        exportStl         = false;
  int         imageResolution   = 500;
  double      deflection        = 0.02; // tessellation BVH deflection (default)
  double      angularDeflection = 0.1;  // radians (default)
  double      newtonTolerance   = 1e-7; // Newton refinement tolerance (default)
  int         newtonMaxIter     = 10;   // Newton max iterations (default)
  bool        useSIMDNewton     = false; // SIMD Newton batching (experimental)
  bool        useROI            = false;
  double      roiX1 = 0, roiY1 = 0, roiX2 = 0, roiY2 = 0;

  // Backend and parallelization options
  BRepIntCurveSurface_BVHBackend backend           = BRepIntCurveSurface_BVHBackend::OCCT_BVH;
  bool                           useOpenMP         = true;  // Enabled by default
  bool                           allowDisconnected = false; // Allow disconnected shapes
  bool                           useCoherent       = false; // Use coherent packet raytracing
  bool                           enableRaySorting  = true;  // Enable ray sorting for coherent tracing

  // NumPy output channel flags
  bool npyPosition  = false; // X, Y, Z position (3 channels)
  bool npyHeight    = false; // Z height only (1 channel)
  bool npyNormals   = false; // Normal X, Y, Z (3 channels)
  bool npyFaceId    = false; // Face ID (1 channel)
  bool npyCurvGauss = false; // Gaussian curvature K (1 channel)
  bool npyCurvMean  = false; // Mean curvature H (1 channel)
  bool npyCurvMin   = false; // Min principal curvature (1 channel)
  bool npyCurvMax   = false; // Max principal curvature (1 channel)

  // Parse arguments
  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help")
    {
      PrintUsage(argv[0]);
      return 0;
    }
    else if (arg == "-o" || arg == "--output-image")
    {
      outputZHeight = true;
    }
    else if (arg == "-N" || arg == "--with-normals")
    {
      outputWithNormals = true;
    }
    else if (arg == "-n" || arg == "--hit-count")
    {
      outputHitCount = true;
    }
    else if (arg == "-b" || arg == "--benchmark")
    {
      runBenchmarks = true;
    }
    else if (arg == "-s" || arg == "--export-stl")
    {
      exportStl = true;
    }
    else if (arg == "-r" || arg == "--resolution")
    {
      if (i + 1 < argc)
      {
        imageResolution = std::atoi(argv[++i]);
        if (imageResolution < 10)
          imageResolution = 10;
        if (imageResolution > 4000)
          imageResolution = 4000;
      }
    }
    else if (arg == "-d" || arg == "--deflection")
    {
      if (i + 1 < argc)
      {
        deflection = std::atof(argv[++i]);
        if (deflection < 0)
          deflection = 0;
      }
    }
    else if (arg == "-a" || arg == "--angle")
    {
      if (i + 1 < argc)
      {
        angularDeflection = std::atof(argv[++i]);
        if (angularDeflection <= 0)
          angularDeflection = 0.5;
      }
    }
    else if (arg == "--newton-tol")
    {
      if (i + 1 < argc)
      {
        newtonTolerance = std::atof(argv[++i]);
        if (newtonTolerance <= 0)
          newtonTolerance = 1e-7;
      }
    }
    else if (arg == "--newton-iter")
    {
      if (i + 1 < argc)
      {
        newtonMaxIter = std::atoi(argv[++i]);
        if (newtonMaxIter < 1)
          newtonMaxIter = 1;
        if (newtonMaxIter > 100)
          newtonMaxIter = 100;
      }
    }
    else if (arg == "--simd-newton")
    {
      useSIMDNewton = true;
    }
    else if (arg == "--roi")
    {
      if (i + 1 < argc)
      {
        const char* roiArg = argv[++i];
        int parsed = sscanf(roiArg, "%lf,%lf,%lf,%lf", &roiX1, &roiY1, &roiX2, &roiY2);
        if (parsed == 4)
        {
          useROI = true;
        }
        else
        {
          std::cerr << "Warning: Failed to parse ROI (expected X1,Y1,X2,Y2 format)" << std::endl;
        }
      }
    }
    // NumPy output channel flags
    else if (arg == "--position")
    {
      npyPosition = true;
    }
    else if (arg == "--height")
    {
      npyHeight = true;
    }
    else if (arg == "--normals")
    {
      npyNormals = true;
    }
    else if (arg == "--faceid")
    {
      npyFaceId = true;
    }
    else if (arg == "--curvature-gauss")
    {
      npyCurvGauss = true;
    }
    else if (arg == "--curvature-mean")
    {
      npyCurvMean = true;
    }
    else if (arg == "--curvature-min")
    {
      npyCurvMin = true;
    }
    else if (arg == "--curvature-max")
    {
      npyCurvMax = true;
    }
    else if (arg == "--curvatures")
    {
      npyCurvGauss = npyCurvMean = npyCurvMin = npyCurvMax = true;
    }
    else if (arg == "--all")
    {
      npyPosition = npyHeight = npyNormals = npyFaceId = true;
      npyCurvGauss = npyCurvMean = npyCurvMin = npyCurvMax = true;
    }
    // Backend and parallelization options
    else if (arg == "--backend")
    {
      if (i + 1 < argc)
      {
        std::string backendArg = argv[++i];
        if (backendArg == "occt")
        {
          backend = BRepIntCurveSurface_BVHBackend::OCCT_BVH;
        }
        else if (backendArg == "embree")
        {
          backend = BRepIntCurveSurface_BVHBackend::Embree_Scalar;
        }
        else if (backendArg == "embree4")
        {
          backend = BRepIntCurveSurface_BVHBackend::Embree_SIMD4;
        }
        else if (backendArg == "embree8")
        {
          backend = BRepIntCurveSurface_BVHBackend::Embree_SIMD8;
        }
        else
        {
          std::cerr << "Warning: Unknown backend '" << backendArg << "', using occt" << std::endl;
        }
      }
    }
    else if (arg == "--openmp")
    {
      useOpenMP = true;
    }
    else if (arg == "--no-openmp")
    {
      useOpenMP = false;
    }
    else if (arg == "--coherent")
    {
      useCoherent = true;
    }
    else if (arg == "--no-sorting")
    {
      enableRaySorting = false;
    }
    else if (arg == "--allow-disconnected")
    {
      allowDisconnected = true;
    }
    else if (arg[0] != '-')
    {
      inputFile = arg;
    }
  }

  // Check that input file is provided
  if (inputFile.empty())
  {
    std::cerr << "Error: No input file specified." << std::endl;
    std::cerr << std::endl;
    PrintUsage(argv[0]);
    return 1;
  }

  TopoDS_Shape shape;

  std::cout << "\nLoading BREP file: " << inputFile << std::endl;
  if (!LoadBREP(inputFile, shape))
    return 1;
  std::cout << "File loaded successfully." << std::endl;

  // Check connected components
  int nbComponents = CountConnectedComponents(shape);
  std::cout << "Connected components: " << nbComponents << std::endl;
  if (nbComponents != 1 && !allowDisconnected)
  {
    std::cerr << "Error: Shape has " << nbComponents << " disconnected face components."
              << std::endl;
    std::cerr << "       Use --allow-disconnected to proceed anyway." << std::endl;
    return 1;
  }
  if (nbComponents != 1 && allowDisconnected)
  {
    std::cout << "Warning: Shape has " << nbComponents << " disconnected components (allowed)."
              << std::endl;
  }

  Bnd_Box bndBox;
  BRepBndLib::Add(shape, bndBox);

  Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
  bndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  std::cout << "\nBounding box:" << std::endl;
  std::cout << "  X: [" << xmin << ", " << xmax << "] (size: " << (xmax - xmin) << ")" << std::endl;
  std::cout << "  Y: [" << ymin << ", " << ymax << "] (size: " << (ymax - ymin) << ")" << std::endl;
  std::cout << "  Z: [" << zmin << ", " << zmax << "] (size: " << (zmax - zmin) << ")" << std::endl;

  std::cout << "\nBuilding BVH acceleration structure..." << std::endl;

  // Tessellate the shape (always done, using default or user-specified deflection)
  std::cout << "Tessellating shape with deflection: " << deflection
            << ", angle: " << angularDeflection << " rad" << std::endl;
  OSD_Timer meshTimer;
  meshTimer.Start();
  BRepMesh_IncrementalMesh mesher(shape, deflection, Standard_False, angularDeflection);
  meshTimer.Stop();
  std::cout << "Tessellation completed in " << std::fixed << std::setprecision(2)
            << meshTimer.ElapsedTime() * 1000.0 << " ms" << std::endl;

  // Export tessellation as STL if requested
  if (exportStl)
  {
    std::string outDir   = inputFile.empty() ? "." : GetDirectory(inputFile);
    std::string baseName = GetBasename(inputFile);
    std::string stlPath  = outDir + "/" + baseName + "_tessellation.stl";

    if (WriteSTL(stlPath, shape))
    {
      std::cout << "Tessellation exported to: " << stlPath << std::endl;
    }
  }

  BRepIntCurveSurface_InterBVH raytracer;

  // Configure backend and parallelization
  raytracer.SetBackend(backend);
  raytracer.SetUseOpenMP(useOpenMP);
  raytracer.SetEnableRaySorting(enableRaySorting);
  raytracer.SetNewtonTolerance(newtonTolerance);
  raytracer.SetNewtonMaxIter(newtonMaxIter);
  raytracer.SetUseSIMDNewton(useSIMDNewton);

  OSD_Timer loadTimer;
  loadTimer.Start();
  raytracer.Load(shape, 0.001, deflection);
  loadTimer.Stop();

  std::cout << "BVH built in " << std::fixed << std::setprecision(2)
            << loadTimer.ElapsedTime() * 1000.0 << " ms" << std::endl;
  std::cout << "Number of faces: " << raytracer.NbFaces() << std::endl;

  // Output images if requested
  std::string outDir   = inputFile.empty() ? "." : GetDirectory(inputFile);
  std::string baseName = inputFile.empty() ? "sphere" : GetBasename(inputFile);

  // Create render box (use ROI if specified, otherwise full bounding box)
  Bnd_Box renderBox = bndBox;
  if (useROI)
  {
    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    bndBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    renderBox.SetVoid();
    renderBox.Update(roiX1, roiY1, zmin, roiX2, roiY2, zmax);
    std::cout << "\nUsing ROI: X=[" << roiX1 << ", " << roiX2 << "], Y=[" << roiY1 << ", " << roiY2
              << "]" << std::endl;
  }

  if (outputZHeight)
  {
    std::cout << "\n=== Rendering Z-Height Image ===" << std::endl;
    std::string suffix  = outputWithNormals ? "_zheight_normals.bmp" : "_zheight.bmp";
    std::string outPath = outDir + "/" + baseName + suffix;
    RenderZHeightImage(raytracer, renderBox, imageResolution, outPath, outputWithNormals);
  }

  if (outputHitCount)
  {
    std::cout << "\n=== Rendering Hit Count Image ===" << std::endl;
    RenderHitCountImage(raytracer, renderBox, imageResolution, outDir, baseName);
  }

  // NumPy output if any channels requested
  bool anyNpyOutput = npyPosition || npyHeight || npyNormals || npyFaceId || npyCurvGauss
                      || npyCurvMean || npyCurvMin || npyCurvMax;
  if (anyNpyOutput)
  {
    std::cout << "\n=== Rendering NumPy Output ===" << std::endl;
    std::string outPath = outDir + "/" + baseName + "_data.npy";
    RenderToNumpy(raytracer,
                  renderBox,
                  imageResolution,
                  outPath,
                  npyPosition,
                  npyHeight,
                  npyNormals,
                  npyFaceId,
                  npyCurvGauss,
                  npyCurvMean,
                  npyCurvMin,
                  npyCurvMax);
  }

  // Run benchmarks (only if requested)
  if (runBenchmarks)
  {
    std::cout << "\n=== Batch Raytracing Benchmarks ===" << std::endl;
    std::cout << "Ray grid covering bounding box, shooting -Z direction" << std::endl;
    if (useCoherent)
    {
      std::cout << "Using COHERENT packet raytracing" << std::endl;
    }
    std::cout << std::string(65, '-') << std::endl;

    std::vector<Standard_Integer> resolutions = {10, 32, 64, 100, 200, 500, 1000};

    for (Standard_Integer res : resolutions)
    {
      RunBenchmark(raytracer, res, bndBox, useCoherent);
    }

    std::cout << std::string(65, '-') << std::endl;
  }

  std::cout << "\nDone!" << std::endl;

  return 0;
}
