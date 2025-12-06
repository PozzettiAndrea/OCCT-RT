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
option(BUILD_RAYTRACER "Build the raytracer command-line tool" ON)
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
# HELPER: Find OCCT library by name (cross-platform)
# =============================================================================
function(find_occt_library VAR_NAME LIB_NAME)
    find_library(${VAR_NAME}
        NAMES ${LIB_NAME}
        PATHS ${OpenCASCADE_LIBRARY_DIR}
        NO_DEFAULT_PATH
    )
    if(NOT ${VAR_NAME})
        message(FATAL_ERROR "Could not find OCCT library: ${LIB_NAME}")
    endif()
endfunction()

# =============================================================================
# FIND OCCT COMPONENT LIBRARIES
# =============================================================================
# Core libraries needed for OCCT_RT
find_occt_library(OCCT_TKTopAlgo TKTopAlgo)
find_occt_library(OCCT_TKBRep TKBRep)
find_occt_library(OCCT_TKMath TKMath)
find_occt_library(OCCT_TKernel TKernel)
find_occt_library(OCCT_TKG3d TKG3d)
find_occt_library(OCCT_TKGeomBase TKGeomBase)
find_occt_library(OCCT_TKMesh TKMesh)

# Additional libraries for raytracer tool
find_occt_library(OCCT_TKDESTEP TKDESTEP)
find_occt_library(OCCT_TKDEIGES TKDEIGES)
find_occt_library(OCCT_TKDE TKDE)
find_occt_library(OCCT_TKShHealing TKShHealing)
find_occt_library(OCCT_TKPrim TKPrim)

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

# Link against OCCT libraries (cross-platform)
target_link_libraries(OCCT_RT PUBLIC
    ${OCCT_TKTopAlgo}
    ${OCCT_TKBRep}
    ${OCCT_TKMath}
    ${OCCT_TKernel}
    ${OCCT_TKG3d}
    ${OCCT_TKGeomBase}
    ${OCCT_TKMesh}
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
    # Try Embree 4 first, then fall back to Embree 3
    find_package(embree 4 QUIET CONFIG)
    if(NOT embree_FOUND)
        find_package(embree 3 QUIET CONFIG)
    endif()
    if(embree_FOUND)
        target_link_libraries(OCCT_RT PUBLIC embree)
        target_compile_definitions(OCCT_RT PUBLIC OCCT_USE_EMBREE)
        message(STATUS "Embree ${embree_VERSION} found - using Embree BVH acceleration")
    else()
        message(STATUS "Embree not found - using OCCT built-in BVH")
    endif()
endif()

# =============================================================================
# RAYTRACER COMMAND-LINE TOOL
# =============================================================================
if(BUILD_RAYTRACER)
    message(STATUS "Building raytracer command-line tool...")

    add_executable(raytracer
        ${PROJECT_SOURCE_DIR}/tools/raytracer.cxx
    )

    target_link_libraries(raytracer PRIVATE
        OCCT_RT
        ${OCCT_TKDESTEP}
        ${OCCT_TKDEIGES}
        ${OCCT_TKDE}
        ${OCCT_TKShHealing}
        ${OCCT_TKPrim}
    )

    target_include_directories(raytracer PRIVATE
        ${PROJECT_SOURCE_DIR}/src/BRepIntCurveSurface
        ${OpenCASCADE_INCLUDE_DIR}
    )

    set_target_properties(raytracer PROPERTIES
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

if(BUILD_RAYTRACER)
    install(TARGETS raytracer
        RUNTIME DESTINATION bin
    )
endif()

message(STATUS "=== OCCT-RT configuration complete ===")
