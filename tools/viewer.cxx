// OCCT-RT Interactive Viewer
// CPU raytraced CAD viewer with trackball navigation
//
// Usage: occt-rt-viewer [options] <file.step|file.iges|file.brep>
//
// Controls:
//   Left-drag    Rotate (trackball)
//   Scroll       Zoom
//   Middle-drag  Pan
//   R            Reset view
//   Q/Escape     Quit
//
// Copyright (c) 2024 Andrea Pozzetti
// Licensed under LGPL 2.1

#include <Viewer_Camera.hxx>
#include <Viewer_CADLoader.hxx>
#include <BRepIntCurveSurface_InterBVH.hxx>

#include <BRepBndLib.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Bnd_Box.hxx>
#include <gp_Lin.hxx>
#include <NCollection_Array1.hxx>
#include <OSD_Timer.hxx>

#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//=============================================================================
// Global State
//=============================================================================

struct ViewerState
{
  // Camera
  Viewer_Camera camera;

  // Raytracer
  BRepIntCurveSurface_InterBVH raytracer;
  TopoDS_Shape shape;
  Bnd_Box boundingBox;

  // Render buffer (RGB)
  std::vector<uint8_t> pixels;
  int width  = 800;
  int height = 600;

  // Interaction state
  bool needsRedraw    = true;
  bool isRotating     = false;
  bool isPanning      = false;
  double lastMouseX   = 0;
  double lastMouseY   = 0;

  // Performance options
  bool lowResPreview   = true;  // Render at lower res during interaction
  int  previewDivisor  = 4;     // Render at 1/4 resolution during drag

  // Light direction for shading (normalized)
  gp_Dir lightDir = gp_Dir(0.5, 0.3, 0.8);

  // Timing
  double lastRenderTime = 0;
};

static ViewerState g_state;

//=============================================================================
// Rendering
//=============================================================================

void Render(bool lowRes)
{
  OSD_Timer timer;
  timer.Start();

  int divisor      = lowRes ? g_state.previewDivisor : 1;
  int renderWidth  = g_state.width / divisor;
  int renderHeight = g_state.height / divisor;

  // Generate rays
  NCollection_Array1<gp_Lin> rays;
  g_state.camera.GenerateAllRays(rays, renderWidth, renderHeight);

  // Raytrace
  NCollection_Array1<BRepIntCurveSurface_HitResult> results;
  g_state.raytracer.PerformBatch(rays, results);

  // Convert to pixels with Lambertian shading
  for (int y = 0; y < renderHeight; ++y)
  {
    for (int x = 0; x < renderWidth; ++x)
    {
      int rayIdx = y * renderWidth + x + 1;
      const auto& hit = results(rayIdx);

      uint8_t r, g, b;
      if (hit.IsValid)
      {
        // Two-sided Lambertian shading
        Standard_Real NdotL = hit.Normal.Dot(g_state.lightDir);
        NdotL = std::abs(NdotL); // Two-sided lighting

        // Ambient + diffuse
        Standard_Real ambient   = 0.15;
        Standard_Real diffuse   = 0.85;
        Standard_Real intensity = ambient + diffuse * NdotL;
        intensity = std::min(1.0, intensity);

        // Light gray material
        uint8_t gray = static_cast<uint8_t>(intensity * 220);
        r = g = b = gray;
      }
      else
      {
        // Background - dark gray
        r = g = b = 40;
      }

      // Write to pixel buffer (upscale if low-res)
      for (int dy = 0; dy < divisor; ++dy)
      {
        for (int dx = 0; dx < divisor; ++dx)
        {
          int px = x * divisor + dx;
          int py = y * divisor + dy;
          if (px < g_state.width && py < g_state.height)
          {
            // OpenGL expects bottom-to-top, so flip Y
            int flippedY = g_state.height - 1 - py;
            int pixIdx   = (flippedY * g_state.width + px) * 3;
            g_state.pixels[pixIdx + 0] = r;
            g_state.pixels[pixIdx + 1] = g;
            g_state.pixels[pixIdx + 2] = b;
          }
        }
      }
    }
  }

  timer.Stop();
  g_state.lastRenderTime = timer.ElapsedTime() * 1000.0; // ms
}

void DisplayPixels()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glRasterPos2i(-1, -1);
  glDrawPixels(g_state.width, g_state.height, GL_RGB, GL_UNSIGNED_BYTE, g_state.pixels.data());
}

//=============================================================================
// GLFW Callbacks
//=============================================================================

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  (void)scancode;
  (void)mods;

  if (action != GLFW_PRESS)
    return;

  switch (key)
  {
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
      glfwSetWindowShouldClose(window, GLFW_TRUE);
      break;

    case GLFW_KEY_R:
      g_state.camera.Reset();
      g_state.needsRedraw = true;
      break;
  }
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  (void)mods;

  double x, y;
  glfwGetCursorPos(window, &x, &y);

  // Normalize to [-1, 1]
  double normX = (2.0 * x / g_state.width) - 1.0;
  double normY = 1.0 - (2.0 * y / g_state.height); // Flip Y

  if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action == GLFW_PRESS)
    {
      g_state.isRotating = true;
      g_state.camera.BeginRotation(normX, normY);
      g_state.lastMouseX = x;
      g_state.lastMouseY = y;
    }
    else if (action == GLFW_RELEASE)
    {
      g_state.isRotating = false;
      g_state.camera.EndRotation();
      g_state.needsRedraw = true; // Full-res redraw
    }
  }
  else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
  {
    if (action == GLFW_PRESS)
    {
      g_state.isPanning = true;
      g_state.lastMouseX = x;
      g_state.lastMouseY = y;
    }
    else if (action == GLFW_RELEASE)
    {
      g_state.isPanning = false;
      g_state.needsRedraw = true;
    }
  }
}

void CursorPosCallback(GLFWwindow* window, double x, double y)
{
  (void)window;

  double normX = (2.0 * x / g_state.width) - 1.0;
  double normY = 1.0 - (2.0 * y / g_state.height);

  if (g_state.isRotating)
  {
    g_state.camera.UpdateRotation(normX, normY);
    g_state.needsRedraw = true;
  }
  else if (g_state.isPanning)
  {
    double dx = (x - g_state.lastMouseX) / g_state.width;
    double dy = (g_state.lastMouseY - y) / g_state.height;
    g_state.camera.Pan(dx * 2.0, dy * 2.0);
    g_state.lastMouseX = x;
    g_state.lastMouseY = y;
    g_state.needsRedraw = true;
  }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  (void)window;
  (void)xoffset;

  g_state.camera.Zoom(yoffset * 0.2);
  g_state.needsRedraw = true;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  (void)window;

  if (width <= 0 || height <= 0)
    return;

  g_state.width  = width;
  g_state.height = height;
  g_state.pixels.resize(width * height * 3);
  g_state.needsRedraw = true;

  glViewport(0, 0, width, height);
}

//=============================================================================
// Argument Parsing
//=============================================================================

void PrintUsage(const char* progName)
{
  std::cout << "OCCT-RT Interactive Viewer\n";
  std::cout << "Usage: " << progName << " [options] <file.step|file.iges|file.brep>\n\n";
  std::cout << "Options:\n";
  std::cout << "  --eye X Y Z       Camera position\n";
  std::cout << "  --lookat X Y Z    Look-at target\n";
  std::cout << "  --up X Y Z        Up vector (default: 0 0 1)\n";
  std::cout << "  --fov DEGREES     Field of view (default: 45)\n";
  std::cout << "  --width W         Window width (default: 800)\n";
  std::cout << "  --height H        Window height (default: 600)\n";
  std::cout << "  --backend B       Raytracer backend: occt|embree|embree4|embree8\n";
  std::cout << "  --deflection D    Tessellation deflection (default: 0.01)\n";
  std::cout << "  --help            Show this help message\n\n";
  std::cout << "Controls:\n";
  std::cout << "  Left-drag    Rotate (trackball)\n";
  std::cout << "  Scroll       Zoom\n";
  std::cout << "  Middle-drag  Pan\n";
  std::cout << "  R            Reset view\n";
  std::cout << "  Q/Escape     Quit\n";
}

BRepIntCurveSurface_BVHBackend ParseBackend(const std::string& str)
{
  if (str == "occt")
    return BRepIntCurveSurface_BVHBackend::OCCT_BVH;
  if (str == "embree")
    return BRepIntCurveSurface_BVHBackend::Embree_Scalar;
  if (str == "embree4")
    return BRepIntCurveSurface_BVHBackend::Embree_SIMD4;
  if (str == "embree8")
    return BRepIntCurveSurface_BVHBackend::Embree_SIMD8;
  return BRepIntCurveSurface_BVHBackend::Embree_SIMD4; // Default
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char* argv[])
{
  // Parse arguments
  std::string inputFile;
  gp_Pnt eye, lookat;
  gp_Dir up(0, 0, 1); // Default up = Z
  Standard_Real fov        = 45.0;
  int windowWidth          = 800;
  int windowHeight         = 600;
  Standard_Real deflection = 0.01;
  BRepIntCurveSurface_BVHBackend backend = BRepIntCurveSurface_BVHBackend::Embree_SIMD4;
  bool hasEye    = false;
  bool hasLookat = false;

  for (int i = 1; i < argc; ++i)
  {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h")
    {
      PrintUsage(argv[0]);
      return 0;
    }
    else if (arg == "--eye" && i + 3 < argc)
    {
      eye    = gp_Pnt(std::stod(argv[i + 1]), std::stod(argv[i + 2]), std::stod(argv[i + 3]));
      hasEye = true;
      i += 3;
    }
    else if (arg == "--lookat" && i + 3 < argc)
    {
      lookat    = gp_Pnt(std::stod(argv[i + 1]), std::stod(argv[i + 2]), std::stod(argv[i + 3]));
      hasLookat = true;
      i += 3;
    }
    else if (arg == "--up" && i + 3 < argc)
    {
      up = gp_Dir(std::stod(argv[i + 1]), std::stod(argv[i + 2]), std::stod(argv[i + 3]));
      i += 3;
    }
    else if (arg == "--fov" && i + 1 < argc)
    {
      fov = std::stod(argv[++i]);
    }
    else if (arg == "--width" && i + 1 < argc)
    {
      windowWidth = std::stoi(argv[++i]);
    }
    else if (arg == "--height" && i + 1 < argc)
    {
      windowHeight = std::stoi(argv[++i]);
    }
    else if (arg == "--deflection" && i + 1 < argc)
    {
      deflection = std::stod(argv[++i]);
    }
    else if (arg == "--backend" && i + 1 < argc)
    {
      backend = ParseBackend(argv[++i]);
    }
    else if (arg[0] != '-')
    {
      inputFile = arg;
    }
  }

  if (inputFile.empty())
  {
    PrintUsage(argv[0]);
    return 1;
  }

  // Load CAD file
  std::cout << "Loading: " << inputFile << std::endl;
  std::string error;
  if (!Viewer_CADLoader::Load(inputFile, g_state.shape, error))
  {
    std::cerr << "Error: " << error << std::endl;
    return 1;
  }
  std::cout << "File loaded successfully (" << Viewer_CADLoader::FormatName(Viewer_CADLoader::DetectFormat(inputFile)) << ")" << std::endl;

  // Validate single component
  if (!Viewer_CADLoader::ValidateSingleComponent(g_state.shape, error))
  {
    std::cerr << "Error: " << error << std::endl;
    return 1;
  }
  std::cout << "Validated: single connected component" << std::endl;

  // Get bounding box
  BRepBndLib::Add(g_state.shape, g_state.boundingBox);

  Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
  g_state.boundingBox.Get(xmin, ymin, zmin, xmax, ymax, zmax);
  std::cout << "Bounding box: [" << xmin << ", " << xmax << "] x ["
            << ymin << ", " << ymax << "] x [" << zmin << ", " << zmax << "]" << std::endl;

  // Tessellate
  std::cout << "Tessellating with deflection: " << deflection << std::endl;
  BRepMesh_IncrementalMesh mesher(g_state.shape, deflection, Standard_False, 0.5);

  // Build BVH
  std::cout << "Building BVH acceleration structure..." << std::endl;
  g_state.raytracer.Load(g_state.shape, 0.001, deflection);
  g_state.raytracer.SetBackend(backend);
  g_state.raytracer.SetComputeCurvature(Standard_False);   // Don't need curvature
  g_state.raytracer.SetNewtonMaxIter(3);                   // Fast but less precise
  g_state.raytracer.SetNewtonTolerance(1e-4);              // Relaxed tolerance
  g_state.raytracer.SetSkipNewtonForPlanar(Standard_True); // Skip Newton for planar faces
  std::cout << "BVH built with " << g_state.raytracer.NbFaces() << " faces" << std::endl;

  // Setup camera
  g_state.camera.SetFOV(fov);
  g_state.camera.SetUp(up);

  if (!hasEye && !hasLookat)
  {
    // Auto-fit to bounding box
    g_state.camera.FitToBounds(g_state.boundingBox, 1.5);
  }
  else
  {
    if (hasEye)
      g_state.camera.SetEye(eye);
    if (hasLookat)
      g_state.camera.SetLookAt(lookat);
    g_state.camera.SaveHomePosition();
  }

  // Initialize GLFW
  if (!glfwInit())
  {
    std::cerr << "Error: Failed to initialize GLFW" << std::endl;
    return 1;
  }

  // Create window
  g_state.width  = windowWidth;
  g_state.height = windowHeight;
  GLFWwindow* window = glfwCreateWindow(g_state.width, g_state.height, "OCCT-RT Viewer", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Error: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // VSync

  // Set callbacks
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);
  glfwSetCursorPosCallback(window, CursorPosCallback);
  glfwSetScrollCallback(window, ScrollCallback);
  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

  // Initialize pixel buffer
  g_state.pixels.resize(g_state.width * g_state.height * 3);

  // Set OpenGL state
  glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  std::cout << "\nViewer ready. Controls:\n";
  std::cout << "  Left-drag: rotate, Scroll: zoom, Middle-drag: pan\n";
  std::cout << "  R: reset view, Q/Escape: quit\n" << std::endl;

  // Render loop
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    if (g_state.needsRedraw)
    {
      bool isInteracting = g_state.isRotating || g_state.isPanning;
      Render(isInteracting && g_state.lowResPreview);
      DisplayPixels();
      glfwSwapBuffers(window);

      // Update window title with render time
      std::string title = "OCCT-RT Viewer - " + std::to_string(static_cast<int>(g_state.lastRenderTime)) + " ms";
      if (isInteracting && g_state.lowResPreview)
        title += " (preview)";
      glfwSetWindowTitle(window, title.c_str());

      if (!isInteracting)
      {
        g_state.needsRedraw = false;
      }
    }
    else
    {
      // No redraw needed - wait for events to save CPU
      glfwWaitEvents();
    }
  }

  // Cleanup
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
