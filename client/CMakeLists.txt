cmake_minimum_required(VERSION 2.6)

project(server)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_COMPILER g++)
add_compile_options(-DICE_CPP11_MAPPING)

set(UVSS_COMM_SDK_VER "test")
configure_file(
        "${PROJECT_SOURCE_DIR}/version.h.in"
        "${PROJECT_BINARY_DIR}/version.h")

include_directories("${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}" "/opt/Ice-3.7a4/include")
link_directories("/opt/Ice-3.7a4/lib/x86_64-linux-gnu" "${PROJECT_BINARY_DIR}")

add_custom_command(
        OUTPUT ${PROJECT_BINARY_DIR}/clientserver.h ${PROJECT_BINARY_DIR}/clientserver.cpp
        COMMAND COMMAND /opt/Ice-3.7a4/bin/slice2cpp -I/opt/Ice-3.7a4/slice ${PROJECT_SOURCE_DIR}/../clientserver.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../clientserver.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp ${PROJECT_SOURCE_DIR}/src/clienti.cpp ${PROJECT_SOURCE_DIR}/src/uvssclient.cpp ${PROJECT_SOURCE_DIR}/src/uvssclientsdk.cpp ${PROJECT_SOURCE_DIR}/src/WorkQueue.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSSDK SHARED ${SOURCES})
target_link_libraries(UVSSSDK Ice++11 IceUtil++11 pthread boost_system boost_filesystem)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(client ${PROJECT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(client UVSSSDK Ice++11 IceUtil++11 pthread boost_system boost_filesystem)
