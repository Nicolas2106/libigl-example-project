cmake_minimum_required(VERSION 3.16)
project(garland)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
include(libigl)

#Bring the headers .h files
include_directories(include)

# Add project's files
file(GLOB SRC_FILES src/*)

add_executable(garland ${SRC_FILES})
target_link_libraries(garland PUBLIC igl::core igl::opengl_glfw)