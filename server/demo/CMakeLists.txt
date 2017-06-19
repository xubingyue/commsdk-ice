IF (UNIX)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(server server.cpp)
target_link_libraries(server UVSSServer)

ELSEIF (WIN32)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}\\bin)
add_executable(server server.cpp)
target_link_libraries(server UVSSServer)

ENDIF ()
