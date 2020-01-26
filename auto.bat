cd %~f0
mkdir build
cd build
conan install .. --build missing
:: cmake .. -G "Visual Studio 16 2019" -A Win64
:: cmake --build . --config Debug
cd ..
