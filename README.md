* A starting point for C++ projects using SDL2.
* Uses conan.io[https://conan.io] to obtain and setup libraries.
* Uses CMake[https://cmake.org] for build system creation.

## Quickstart

Windows:: `auto.bat` #or double-click
Linux:: `./auto.sh`

## Manual start

From a command line, at the root of this project

1. Make a directory for temporary build files
  * `mkdir build`
2. Change into that directory
  * `cd build`
3. Ask conan to install libraries, according to `conanfile.txt`
  * `conan install .. --build missing`
4. Ask CMake to make a build system (build files will be in `build` folder. `.sln` file for Visual Studio)
  * Windows
    - `cmake .. -G "Visual Studio 14 2015 Win64"`
  * Linux
    - `cmake .. -G "Unix Makefiles`
  * Any (auto select build system)
    - `cmake ..`
5. Do the build
  - Visual Studio, manually
    - open the `.sln` file and hit *build*
  - Ask CMake to ask the build system to build (OPTIONAL)
    - `cmake --build . --config Debug`
6. Return to parent directory
  * `cd ..`

## Other resources

- [Template by shearer12345](https://github.com/shearer12345/sdl2template)
- [CMake and Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
- [Conan SDL2 remote](<https://github.com/bincrafters/conan-sdl2>)

