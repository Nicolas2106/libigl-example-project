# Garland example surface simplification project

An example project using libigl about surface simplification and quadric error metrics, which is put forward by Michael Garland and Paul S. Heckbert.

## Dependencies

The only dependencies are STL, Eigen, [libigl](http://libigl.github.io/libigl/) and the dependencies
of the `igl::opengl::glfw::Viewer` (OpenGL, glad and GLFW).
The CMake build system will automatically download libigl and its dependencies using
[CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html),
thus requiring no setup on your part.

To use a local copy of libigl rather than downloading the repository via FetchContent, you can use
the CMake cache variable `FETCHCONTENT_SOURCE_DIR_LIBIGL` when configuring your CMake project for
the first time:
```
cmake -DFETCHCONTENT_SOURCE_DIR_LIBIGL=<path-to-libigl> ..
```
When changing this value, do not forget to clear your `CMakeCache.txt`, or to update the cache variable
via `cmake-gui` or `ccmake`.

## Compile

Compile this project using the standard cmake routine:

    mkdir build
    cd build
    cmake ..
    make

This should find and build the dependencies.

## Run

From within the `build` directory just issue:

    ./garland

Or click on `garland.sln` file to open the Visual Studio project.

## Example of result

Garland simplification example from 6966 faces to 200 faces.

![alt text](https://github.com/Nicolas2106/libigl-example-project/blob/garland/result.png?raw=true)