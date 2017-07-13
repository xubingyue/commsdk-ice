#cmake .. -G "Visual Studio 10 2010 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=false
#cmake .. -G "Visual Studio 10 2010 Win64" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=false
#cmake .. -G "Visual Studio 10 2010" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=false
#cmake .. -G "Visual Studio 10 2010" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=false

#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=true
#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=true
#cmake .. -G "Visual Studio 14 2015" -DCMAKE_CONFIGURATION_TYPES=Debug -DCXX11=true
#cmake .. -G "Visual Studio 14 2015" -DCMAKE_CONFIGURATION_TYPES=Release -DCXX11=true

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
        if(CXX11)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/vc140/x64)
            set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/vc140/x64)
        else(CXX11)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/vc100/x64)
            set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/vc100/x64)
        endif(CXX11)
    else(CMAKE_CL_64)
        if(CXX11)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/vc140/Win32)
            set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/vc140/Win32)
        else(CXX11)
            set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib/vc100/Win32)
            set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/vc100/Win32)
        endif(CXX11)
    endif(CMAKE_CL_64)
endif()

add_subdirectory(src)
add_subdirectory(demo)
