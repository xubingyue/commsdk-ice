#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES:STRING=Debug

cmake_minimum_required(VERSION 2.6)

project(server)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

IF (WIN32)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-DICE_CPP11_MAPPING -DUVSS_COMM_EXPORTS)
set(UVSS_COMM_SDK_VER "test")
configure_file(
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h")

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}" "D:\\Ice-3.7b0\\ice\\cpp\\include" "D:\\Ice-3.7b0\\ice\\cpp\\include\\generated\\cpp11\\x64\\Debug"
"C:\\local\\boost_1_63_0")
link_directories("D:\\Ice-3.7b0\\ice\\cpp\\lib\\x64\\Debug" "${PROJECT_BINARY_DIR}" "C:\\local\\boost_1_63_0\\lib64-msvc-14.0")

add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/clientserver.h ${PROJECT_BINARY_DIR}/clientserver.cpp
        COMMAND COMMAND D:\\Ice-3.7b0\\ice\\cpp\\bin\\x64\\Debug\\slice2cpp -ID:\\Ice-3.7b0\\ice\\slice ${PROJECT_SOURCE_DIR}/../clientserver.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../clientserver.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp ${PROJECT_SOURCE_DIR}/src/serveri.cpp ${PROJECT_SOURCE_DIR}/src/uvssserver.cpp ${PROJECT_SOURCE_DIR}/src/uvssserversdk.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSServer SHARED ${SOURCES})
target_link_libraries(UVSSServer ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_63.lib)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}\\bin)
add_executable(server ${PROJECT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(server UVSSServer ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_63.lib)

ELSEIF (UNIX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER g++)
add_compile_options(-DICE_CPP11_MAPPING)

set(UVSS_COMM_SDK_VER "test")
configure_file(
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h")

include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}")
link_directories("/usr/lib/x86_64-linux-gnu" "${PROJECT_BINARY_DIR}")

add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/clientserver.h ${PROJECT_BINARY_DIR}/clientserver.cpp
        COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../clientserver.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../clientserver.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp ${PROJECT_SOURCE_DIR}/src/serveri.cpp ${PROJECT_SOURCE_DIR}/src/uvssserver.cpp ${PROJECT_SOURCE_DIR}/src/uvssserversdk.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSServer SHARED ${SOURCES})
target_link_libraries(UVSSServer Ice++11 pthread boost_filesystem)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(server ${PROJECT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(server UVSSServer Ice++11 pthread boost_filesystem)

ENDIF ()
