add_compile_options(-DICE_CPP11_MAPPING)

IF (UNIX)

include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
link_directories("/usr/lib/x86_64-linux-gnu")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
        COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbackreceiveri.cpp uvssclient.cpp uvssclientwrapper.cpp workqueue.cpp rpcproxies.cpp global.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSSDK SHARED ${SOURCES})
target_link_libraries(UVSSSDK Ice++11 pthread boost_filesystem)

ELSEIF (WIN32)

add_compile_options(-DUVSS_EXPORTS)

include_directories("D:\\Ice-3.7b0\\ice\\cpp\\include" "D:\\Ice-3.7b0\\ice\\cpp\\include\\generated\\cpp11\\x64\\Debug" "C:\\local\\boost_1_63_0" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
link_directories("D:\\Ice-3.7b0\\ice\\cpp\\lib\\x64\\Debug" "C:\\local\\boost_1_63_0\\lib64-msvc-14.0")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
        COMMAND COMMAND D:\\Ice-3.7b0\\ice\\cpp\\bin\\x64\\Debug\\slice2cpp -ID:\\Ice-3.7b0\\ice\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbackreceiveri.cpp uvssclient.cpp uvssclientwrapper.cpp WorkQueue.cpp rpcproxies.cpp)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
add_library(UVSSSDK SHARED ${SOURCES})
target_link_libraries(UVSSSDK ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_63.lib)

ENDIF ()
