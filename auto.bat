cd %~f0
mkdir build
cd build
conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
conan install .. --build missing
:: cmake .. -G "Visual Studio 16 2019" -A Win64
:: cmake --build . --config Debug
cd ..
