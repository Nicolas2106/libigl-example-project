cmake_minimum_required(VERSION 3.16)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
include(libigl)

# First project
add_executable(mesh_decimation1 simple_decimation.cpp)
target_link_libraries(mesh_decimation1 PUBLIC igl::core igl::opengl_glfw)

# First project
add_executable(mesh_decimation2 demo.cpp)
target_link_libraries(mesh_decimation2 PUBLIC igl::core igl::opengl_glfw)
