add_compile_options(-DICE_CPP11_MAPPING)

IF (UNIX)

include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
link_directories("/usr/lib/x86_64-linux-gnu")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
        COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbackreceiveri.cpp uvssclient.cpp uvssclientwrapper.cpp workqueue.cpp rpcproxies.cpp global.cpp)

add_library(UVSSSDK SHARED ${SOURCES})
target_link_libraries(UVSSSDK Ice++11 pthread boost_filesystem)

ELSEIF (WIN32)

add_compile_options(-DUVSS_EXPORTS)

include_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-14.0-64" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\x64\\Debug" "C:\\local\\boost_1_62_0-msvc-14.0-64\\lib64-msvc-14.0")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
        COMMAND COMMAND C:\\local\\zeroc.ice.v140.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
        DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbackreceiveri.cpp uvssclient.cpp uvssclientwrapper.cpp WorkQueue.cpp rpcproxies.cpp global.cpp)

add_library(UVSSSDKd SHARED ${SOURCES})
target_link_libraries(UVSSSDKd ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_62.lib)

ENDIF ()
