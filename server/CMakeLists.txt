#cmake .. -G "Visual Studio 12 2013 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=false
#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug

cmake_minimum_required(VERSION 2.6)

project(server)

if (NOT CXX11)
    set(CXX11 false)
endif()

if(UNIX)
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
elseif(WIN32)
    if(CMAKE_CL_64)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/x64)
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/x64)
    else(CMAKE_CL_64)
        set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/Win32)
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/Win32)
    endif(CMAKE_CL_64)
endif()

add_subdirectory(src)
add_subdirectory(demo)
