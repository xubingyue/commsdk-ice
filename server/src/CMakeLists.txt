if(UNIX)
    if(CXX11)
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
        target_link_libraries(UVSSServer Ice++11 boost_date_time boost_filesystem pthread)
    else(CXX11)
        set(CMAKE_CXX_STANDARD 98)

        include_directories("/usr/include" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")
        link_directories("/usr/lib/x86_64-linux-gnu")

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

        set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

        add_library(UVSSServer SHARED ${SOURCES})
        target_link_libraries(UVSSServer Ice boost_chrono boost_date_time boost_filesystem boost_thread pthread)
    endif(CXX11)
elseif(WIN32)
    set(CMAKE_DEBUG_POSTFIX d)

    if(CXX11)
        add_compile_options(-DICE_CPP11_MAPPING)
        add_compile_options(-DUVSS_EXPORTS)

        if(CMAKE_CL_64)
            include_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-14.0-64" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

            link_directories("C:\\local\\boost_1_62_0-msvc-14.0-64\\lib64-msvc-14.0")
            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\x64\\Debug")
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\x64\\Release")
            endif()

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v140.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

            add_library(UVSSServer SHARED ${SOURCES})
            target_link_libraries(UVSSServer debug ice37b0++11d.lib optimized ice37b0++11.lib)
        else(CMAKE_CL_64)
            include_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-14.0-32" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

            link_directories("C:\\local\\boost_1_62_0-msvc-14.0-32\\lib32-msvc-14.0")
            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\Win32\\Debug")
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories("C:\\local\\zeroc.ice.v140.3.7.0-beta0\\build\\native\\lib\\Win32\\Release")
            endif()

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v140.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

            add_library(UVSSServer SHARED ${SOURCES})
            target_link_libraries(UVSSServer debug ice37b0++11d.lib optimized ice37b0++11.lib)
        endif(CMAKE_CL_64)
    else(CXX11)
        add_compile_options(-DUVSS_EXPORTS)

        if(CMAKE_CL_64)
            include_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-12.0-64" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

            link_directories("C:\\local\\boost_1_62_0-msvc-12.0-64\\lib64-msvc-12.0")
            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\lib\\x64\\Debug")
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\lib\\x64\\Release")
            endif()

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v120.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v120.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

            add_library(UVSSServer SHARED ${SOURCES})
            target_link_libraries(UVSSServer debug ice37b0d.lib optimized ice37b0.lib)
        else(CMAKE_CL_64)
            include_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\include" "C:\\local\\boost_1_62_0-msvc-12.0-32" "${PROJECT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}")

            link_directories("C:\\local\\boost_1_62_0-msvc-12.0-32\\lib32-msvc-12.0")
            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\lib\\Win32\\Debug")
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories("C:\\local\\zeroc.ice.v120.3.7.0-beta0\\build\\native\\lib\\Win32\\Release")
            endif()

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v120.3.7.0-beta0\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v120.3.7.0-beta0\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp callbacksenderi.cpp uvssserver.cpp uvssserverwrapper.cpp rpcproxies.cpp global.cpp)

            add_library(UVSSServer SHARED ${SOURCES})
            target_link_libraries(UVSSServer debug ice37b0d.lib optimized ice37b0.lib)
        endif(CMAKE_CL_64)
    endif(CXX11)
endif()
