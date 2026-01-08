cmake_minimum_required(VERSION 3.26)
project(fractal_renderer LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)
set(POSITION_INDEPENDENT_CODE ON)

find_package(CUDAToolkit REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
message(STATUS "Qt6_FOUND = ${Qt6_FOUND}")

qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src)
