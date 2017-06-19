#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES:STRING=Debug

cmake_minimum_required(VERSION 2.6)
MESSAGE(STATUS "OS: ${CMAKE_SYSTEM}")

project(server)

add_subdirectory(src)
add_subdirectory(demo)
