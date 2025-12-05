# OCCT-RT: High-Performance BVH Raytracer Extension for OpenCASCADE
# Copyright (c) 2024-2025 Andrea Pozzetti
# LGPL-2.1 with OCCT Exception

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)
project(OCCT-RT VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "=== OCCT-RT: BVH Raytracer for OpenCASCADE ===")

# =============================================================================
# OPTIONS
# =============================================================================
option(BUILD_RAYTRACER_TEST "Build the raytracer test executable" ON)
option(OCCT_RT_USE_OPENMP "Enable OpenMP for parallel ray processing" ON)
option(OCCT_RT_USE_EMBREE "Enable Embree for SIMD ray-triangle intersection" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# =============================================================================
# FIND OPENCASCADE
# =============================================================================
# Try to find OpenCASCADE via cmake config (conda/system install)
find_package(OpenCASCADE CONFIG QUIET)

if(NOT OpenCASCADE_FOUND)
    # Try common installation paths
    find_package(OpenCASCADE CONFIG QUIET
        PATHS
            /usr/local/lib/cmake/opencascade
            /opt/opencascade/lib/cmake/opencascade
            $ENV{CONDA_PREFIX}/lib/cmake/opencascade
    )
endif()

if(OpenCASCADE_FOUND)
    message(STATUS "Found OpenCASCADE ${OpenCASCADE_VERSION} at ${OpenCASCADE_LIBRARY_DIR}")
else()
    message(FATAL_ERROR "OpenCASCADE not found! Install via conda: conda install -c conda-forge occt")
endif()

# =============================================================================
# OCCT-RT LIBRARY
# =============================================================================
message(STATUS "Building OCCT-RT library...")

set(OCCT_RT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_InterBVH.cxx
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_ZEvaluator.cxx
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_OverlapAnalyzer.cxx
)

set(OCCT_RT_HEADERS
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_InterBVH.hxx
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_ZEvaluator.hxx
    ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface/BRepIntCurveSurface_OverlapAnalyzer.hxx
)

add_library(OCCT_RT ${OCCT_RT_SOURCES})

target_include_directories(OCCT_RT PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface>
    $<INSTALL_INTERFACE:include/opencascade>
    ${OpenCASCADE_INCLUDE_DIR}
)

# Link against OCCT libraries
target_link_libraries(OCCT_RT PUBLIC
    ${OpenCASCADE_LIBRARY_DIR}/libTKTopAlgo.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKBRep.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKMath.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKernel.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKG3d.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKGeomBase.so
    ${OpenCASCADE_LIBRARY_DIR}/libTKMesh.so
)

# =============================================================================
# OPTIONAL: OpenMP SUPPORT
# =============================================================================
if(OCCT_RT_USE_OPENMP)
    find_package(OpenMP QUIET)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(OCCT_RT PUBLIC OpenMP::OpenMP_CXX)
        message(STATUS "OpenMP found - enabling parallel ray processing")
    else()
        message(STATUS "OpenMP not found - single-threaded ray processing")
    endif()
endif()

# =============================================================================
# OPTIONAL: EMBREE SUPPORT
# =============================================================================
if(OCCT_RT_USE_EMBREE)
    find_package(embree 3 QUIET CONFIG)
    if(embree_FOUND)
        target_link_libraries(OCCT_RT PUBLIC embree)
        target_compile_definitions(OCCT_RT PUBLIC OCCT_USE_EMBREE)
        message(STATUS "Embree ${embree_VERSION} found - using Embree BVH acceleration")
    else()
        message(STATUS "Embree not found - using OCCT built-in BVH")
    endif()
endif()

# =============================================================================
# RAYTRACER TEST EXECUTABLE
# =============================================================================
if(BUILD_RAYTRACER_TEST)
    message(STATUS "Building raytracer_test executable...")

    add_executable(raytracer_test
        ${PROJECT_SOURCE_DIR}/samples/raytracer/raytracer_test.cxx
    )

    target_link_libraries(raytracer_test PRIVATE
        OCCT_RT
        ${OpenCASCADE_LIBRARY_DIR}/libTKDESTEP.so
        ${OpenCASCADE_LIBRARY_DIR}/libTKDEIGES.so
        ${OpenCASCADE_LIBRARY_DIR}/libTKDE.so
        ${OpenCASCADE_LIBRARY_DIR}/libTKShHealing.so
        ${OpenCASCADE_LIBRARY_DIR}/libTKPrim.so
    )

    target_include_directories(raytracer_test PRIVATE
        ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface
        ${OpenCASCADE_INCLUDE_DIR}
    )

    set_target_properties(raytracer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# =============================================================================
# INSTALL RULES
# =============================================================================
install(TARGETS OCCT_RT
    EXPORT OCCT_RTTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${OCCT_RT_HEADERS}
    DESTINATION include/opencascade
)

install(EXPORT OCCT_RTTargets
    FILE OCCT_RTConfig.cmake
    NAMESPACE OCCT_RT::
    DESTINATION lib/cmake/OCCT_RT
)

message(STATUS "=== OCCT-RT configuration complete ===")
