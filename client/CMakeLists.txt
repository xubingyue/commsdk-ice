#cmake .. -G "Visual Studio 12 2013 Win64" -DCMAKE_BUILD_TYPE=Debug
#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_BUILD_TYPE=Debug

cmake_minimum_required(VERSION 2.6)

project(client)

set(EVO true)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
add_subdirectory(demo)
