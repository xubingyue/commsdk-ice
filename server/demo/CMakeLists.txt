if(UNIX)
    include_directories(${PROJECT_SOURCE_DIR}/include)

    add_executable(server-demo server-demo.cpp)
    target_link_libraries(server-demo uvssserver)
elseif(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/include)

    if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
        link_directories(${LIBRARY_OUTPUT_PATH}/Debug)
    elseif(CMAKE_CONFIGURATION_TYPES STREQUAL "Release")
        link_directories(${LIBRARY_OUTPUT_PATH}/Release)
    endif()

    add_executable(server-demo server-demo.cpp)
    target_link_libraries(server-demo debug uvssserverd.lib optimized uvssserver.lib)
endif()
