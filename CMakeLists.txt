project("mosaic")
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

include(${PROJECT_SOURCE_DIR}/build/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${PROJECT_SOURCE_DIR}/include/*.h)
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

file(COPY assets/ DESTINATION bin/assets)
