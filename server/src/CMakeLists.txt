if(UNIX)

if(EVO)
    set(CMAKE_CXX_STANDARD 11)
    add_compile_options(-DICE_CPP11_MAPPING)

    include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
    link_directories("/usr/lib/x86_64-linux-gnu")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

    set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

    add_library(UVSSServer SHARED ${SOURCES})
    target_link_libraries(UVSSServer Ice++11 pthread boost_filesystem boost_date_time)
else(EVO)
    set(CMAKE_CXX_STANDARD 98)

    include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
    link_directories("/usr/lib/x86_64-linux-gnu")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

    set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

    add_library(UVSSServer SHARED ${SOURCES})
    target_link_libraries(UVSSServer Ice pthread boost_filesystem boost_date_time boost_thread boost_chrono)
endif(EVO)

elseif(WIN32)

if(EVO)
    add_compile_options(-DICE_CPP11_MAPPING)
    add_compile_options(-DUVSS_EXPORTS)

    include_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-14.0-64" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
    link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\x64\\Debug" "C:\\local\\boost_1_62_0-msvc-14.0-64\\lib64-msvc-14.0")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND C:\\local\\zeroc.ice.v140.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

    set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

    add_library(UVSSServerd SHARED ${SOURCES})
    target_link_libraries(UVSSServerd ice37b0++11d.lib boost_filesystem-vc140-mt-gd-1_62.lib boost_date_time-vc140-mt-gd-1_62.lib)

else(EVO)
    add_compile_options(-DUVSS_EXPORTS)

    include_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-12.0-64" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
    link_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\lib\\x64\\Debug" "C:\\local\\boost_1_62_0-msvc-12.0-64\\lib64-msvc-12.0")

    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND C:\\local\\zeroc.ice.v120.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v120.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

    set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

    add_library(UVSSServerd SHARED ${SOURCES})
    target_link_libraries(UVSSServerd ice37b0d.lib boost_filesystem-vc120-mt-gd-1_62.lib boost_date_time-vc120-mt-gd-1_62.lib libboost_thread-vc120-mt-gd-1_62.lib libboost_chrono-vc120-mt-gd-1_62.lib)
endif(EVO)

endif ()
