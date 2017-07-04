if(UNIX)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(client client.cpp)
target_link_libraries(client UVSSSDK)

elseif(WIN32)

include_directories("${PROJECT_SOURCE_DIR}/include")
link_directories("${LIBRARY_OUTPUT_PATH}/Debug")

add_executable(client client.cpp)
target_link_libraries(client UVSSSDKd.lib)

endif()
