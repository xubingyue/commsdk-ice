#cmake .. -G "Visual Studio 14 2015 Win64" -DCMAKE_CONFIGURATION_TYPES:STRING=Debug

cmake_minimum_required(VERSION 2.6)

project(server)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_COMPILER g++)
#add_compile_options(-DICE_CPP11_MAPPING)
add_compile_options(-DICE_CPP11_MAPPING -DUVSS_COMM_EXPORTS)
set(UVSS_COMM_SDK_VER "test")
configure_file(
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h")

#include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}" "/opt/Ice-3.7b0/include")
include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}" "D:\\Ice-3.7b0\\ice\\cpp\\include" "D:\\Ice-3.7b0\\ice\\cpp\\include\\generated\\cpp11\\x64\\Debug"
"C:\\local\\boost_1_63_0")
#link_directories("/opt/Ice-3.7b0/lib/x86_64-linux-gnu" "${PROJECT_BINARY_DIR}")
link_directories("D:\\Ice-3.7b0\\ice\\cpp\\lib\\x64\\Debug" "${PROJECT_BINARY_DIR}" "C:\\local\\boost_1_63_0\\lib64-msvc-14.0")

add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/clientserver.h ${PROJECT_BINARY_DIR}/clientserver.cpp
        #COMMAND COMMAND /opt/Ice-3.7b0/bin/slice2cpp -I/opt/Ice-3.7b0/slice ${PROJECT_SOURCE_DIR}/../clientserver.ice
        COMMAND COMMAND D:\\Ice-3.7b0\\ice\\cpp\\bin\\x64\\Debug\\slice2cpp -ID:\\Ice-3.7b0\\ice\\slice ${PROJECT_SOURCE_DIR}/../clientserver.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../clientserver.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp ${PROJECT_SOURCE_DIR}/src/serveri.cpp ${PROJECT_SOURCE_DIR}/src/uvssserver.cpp ${PROJECT_SOURCE_DIR}/src/uvssserversdk.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSServer SHARED ${SOURCES})
#target_link_libraries(UVSSServer Ice++11 IceUtil++11 pthread boost_system boost_filesystem)
target_link_libraries(UVSSServer ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_63.lib)

#link_directories("${LIBRARY_OUTPUT_PATH}\\Debug")
#link_directories("D:\\Desktop\\LINUX_uvsscommsdk_3.7b0\\server\\lib\\Debug")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}\\bin)
add_executable(server ${PROJECT_SOURCE_DIR}/test/main.cpp)
#target_link_libraries(server UVSSServer Ice++11 IceUtil++11 pthread boost_system boost_filesystem)
target_link_libraries(server UVSSServer ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_63.lib)
