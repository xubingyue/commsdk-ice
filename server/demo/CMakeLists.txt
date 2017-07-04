if(UNIX)
    include_directories("${PROJECT_SOURCE_DIR}/include")

    add_executable(server server.cpp)
    target_link_libraries(server UVSSServer)
elseif(WIN32)
    include_directories("${PROJECT_SOURCE_DIR}/include")
    link_directories("${LIBRARY_OUTPUT_PATH}/Debug")

    add_executable(server server.cpp)
    target_link_libraries(server UVSSServerd.lib)
endif()
