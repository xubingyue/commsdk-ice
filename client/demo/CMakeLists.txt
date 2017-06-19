IF (UNIX)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(client client.cpp)
target_link_libraries(client UVSSSDK)

ELSEIF (WIN32)

include_directories("${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}\\bin)
add_executable(client client.cpp)
target_link_libraries(client UVSSSDK)

ENDIF ()
