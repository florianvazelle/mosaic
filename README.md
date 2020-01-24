* A starting point for C++ projects using SDL2.
* Uses [conan.io](https://conan.io) to obtain and setup libraries.
```sh
pip3 install conan
```
* Uses [CMake](https://cmake.org) for build system creation.

## Quickstart

Windows : `auto.bat` # or double-click

Linux : `./auto.sh`

## Manual start

From a command line, at the root of this project.

Make a directory for temporary build files.

```sh
mkdir build
```

Change into that directory
```sh
cd build
```
Ask conan to install libraries, according to `conanfile.txt`
```sh
conan install .. --build missing
```

Ask CMake to make a build system (build files will be in `build` folder. `.sln` file for Visual Studio)
  * Windows
```sh
cmake .. -G "Visual Studio 14 2015 Win64"
```
  * Linux
```sh
cmake .. -G "Unix Makefiles
```
  * Any (auto select build system)
```sh
cmake ..
```

Do the build
  - Visual Studio, manually
    - open the directory and hit *build*
  - Ask CMake to ask the build system to build (OPTIONAL)
```sh
cmake --build . --config Debug
```

## Other resources

- [Template by shearer12345](https://github.com/shearer12345/sdl2template)
- [CMake and Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)
- [Conan SDL2 remote](<https://github.com/bincrafters/conan-sdl2>)

