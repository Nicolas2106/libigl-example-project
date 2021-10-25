cmake_minimum_required(VERSION 3.16)
project(garland)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
include(libigl)

# Add your project files
file(GLOB SRC_FILES *.cpp)
add_executable(garland garland_simplification.cpp)
target_link_libraries(garland PUBLIC igl::core igl::opengl_glfw)
