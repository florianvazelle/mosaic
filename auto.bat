cd %~f0
mkdir build
cd build
conan install .. --build missing
:: cmake .. -G "Visual Studio 14 2015 Win64"
:: cmake --build . --config Debug
cd ..
