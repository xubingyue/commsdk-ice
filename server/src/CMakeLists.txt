if(UNIX)
    if(CXX11)
        set(CMAKE_CXX_STANDARD 11)
        add_definitions(-DICE_CPP11_MAPPING)

        include_directories(/usr/include ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
        link_directories(/usr/lib/x86_64-linux-gnu)

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

        set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
        aux_source_directory(. SRCS)

        add_library(UVSSServer SHARED ${SRCS})
        target_link_libraries(UVSSServer Ice++11 boost_date_time boost_filesystem pthread)
    else()
        set(CMAKE_CXX_STANDARD 98)

        include_directories(/usr/include ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
        link_directories(/usr/lib/x86_64-linux-gnu)

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
            COMMAND COMMAND /usr/bin/slice2cpp -I/usr/share/ice/slice ${PROJECT_SOURCE_DIR}/../callback.ice
            DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

        set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
        aux_source_directory(. SRCS)

        add_library(UVSSServer SHARED ${SRCS})
        target_link_libraries(UVSSServer Ice boost_chrono boost_date_time boost_filesystem boost_thread pthread)
    endif()
elseif(WIN32)
    set(CMAKE_DEBUG_POSTFIX d)
    add_definitions(-DUVSS_EXPORTS)

    if(CMAKE_CL_64)
        if(CXX11)
            add_definitions(-DICE_CPP11_MAPPING)

            include_directories(C:\\local\\zeroc.ice.v140\\build\\native\\include C:\\local\\boost_1_62_0-msvc-14.0-64 ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories(C:\\local\\zeroc.ice.v140\\build\\native\\lib\\x64\\Debug)
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories(C:\\local\\zeroc.ice.v140\\build\\native\\lib\\x64\\Release)
            endif()
            link_directories(C:\\local\\boost_1_62_0-msvc-14.0-64\\lib64-msvc-14.0)

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v140\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
            aux_source_directory(. SRCS)

            add_library(UVSSServer SHARED ${SRCS})
            target_link_libraries(UVSSServer debug ice37++11d.lib optimized ice37++11.lib)
        else()
            include_directories(C:\\local\\zeroc.ice.v100\\build\\native\\include C:\\local\\boost_1_62_0-msvc-10.0-64 ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories(C:\\local\\zeroc.ice.v100\\build\\native\\lib\\x64\\Debug)
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories(C:\\local\\zeroc.ice.v100\\build\\native\\lib\\x64\\Release)
            endif()
            link_directories(C:\\local\\boost_1_62_0-msvc-10.0-64\\lib64-msvc-10.0)

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v100\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v100\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
            aux_source_directory(. SRCS)

            add_library(UVSSServer SHARED ${SRCS})
            target_link_libraries(UVSSServer debug ice37d.lib optimized ice37.lib)
        endif()
    else()
        if(CXX11)
            add_definitions(-DICE_CPP11_MAPPING)

            include_directories(C:\\local\\zeroc.ice.v140\\build\\native\\include C:\\local\\boost_1_62_0-msvc-14.0-32 ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories(C:\\local\\zeroc.ice.v140\\build\\native\\lib\\Win32\\Debug)
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories(C:\\local\\zeroc.ice.v140\\build\\native\\lib\\Win32\\Release)
            endif()
            link_directories(C:\\local\\boost_1_62_0-msvc-14.0-32\\lib32-msvc-14.0)

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v140\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v140\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
            aux_source_directory(. SRCS)

            add_library(UVSSServer SHARED ${SRCS})
            target_link_libraries(UVSSServer debug ice37++11d.lib optimized ice37++11.lib)
        else()
            include_directories(C:\\local\\zeroc.ice.v100\\build\\native\\include C:\\local\\boost_1_62_0-msvc-10.0-32 ${PROJECT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

            if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
                link_directories(C:\\local\\zeroc.ice.v100\\build\\native\\lib\\Win32\\Debug)
            elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
                link_directories(C:\\local\\zeroc.ice.v100\\build\\native\\lib\\Win32\\Release)
            endif()
            link_directories(C:\\local\\boost_1_62_0-msvc-10.0-32\\lib32-msvc-10.0)

            add_custom_command(
                OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/callback.h ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp
                COMMAND COMMAND C:\\local\\zeroc.ice.v100\\tools\\slice2cpp -IC:\\local\\zeroc.ice.v100\\slice ${PROJECT_SOURCE_DIR}/../callback.ice
                DEPENDS ${PROJECT_SOURCE_DIR}/../callback.ice)

            set(SRCS ${CMAKE_CURRENT_BINARY_DIR}/callback.cpp)
            aux_source_directory(. SRCS)

            add_library(UVSSServer SHARED ${SRCS})
            target_link_libraries(UVSSServer debug ice37d.lib optimized ice37.lib)
        endif()
    endif()
endif()
