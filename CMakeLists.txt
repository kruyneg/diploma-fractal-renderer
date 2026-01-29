cmake_minimum_required(VERSION 3.26)
project(fractal_renderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(POSITION_INDEPENDENT_CODE ON)

option(ENABLE_CUDA "Build CUDA renderer implementation" ON)

if(ENABLE_CUDA)
    enable_language(CUDA)

    set(CMAKE_CUDA_STANDARD 20)
    set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

    find_package(CUDAToolkit REQUIRED)
    message(STATUS "✅ CUDA backend enabled")
else()
    message(STATUS "⚠️ CUDA backend disabled")
endif()

find_package(OpenGL REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
message(STATUS "Qt6_FOUND = ${Qt6_FOUND}")

qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)
