#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES:STRING=Debug

cmake_minimum_required(VERSION 2.6)
message(STATUS "OS: ${CMAKE_SYSTEM}")

project(client)

set(EVO ture)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
add_subdirectory(demo)
