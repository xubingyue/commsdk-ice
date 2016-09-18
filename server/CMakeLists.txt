#-DCMAKE_CONFIGURATION_TYPES:STRING=Debug
#-DCMAKE_CONFIGURATION_TYPES:STRING=Release
#-DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013 Win64"
#-DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013 Win64"

cmake_minimum_required(VERSION 2.6)

project(server)

string(REGEX MATCH "Win64" WIN64_GENERATOR ${CMAKE_GENERATOR})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "C:/ZeroC/Ice-3.6.2/include")

if(WIN64_GENERATOR STREQUAL "Win64")
	link_directories("C:/ZeroC/Ice-3.6.2/lib/x64")
else()
	link_directories("C:/ZeroC/Ice-3.6.2/lib")
endif()

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/clientserver.h ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp
    COMMAND COMMAND slice2cpp -IC:/ZeroC/Ice-3.6.2/slice ${CMAKE_CURRENT_SOURCE_DIR}/../clientserver.ice
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../clientserver.ice
	)

set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/clientserver.h serverI.h uvssserver.h uvssserversdk.h)
set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp serverI.cpp uvssserver.cpp uvssserversdk.cpp)

if(NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX d)
endif()

add_library(UVSSServer SHARED ${HEADERS} ${SOURCES})

target_link_libraries(UVSSServer debug "iced.lib" optimized "ice.lib")
target_link_libraries(UVSSServer debug "iceutild.lib" optimized "iceutil.lib")

string(REGEX REPLACE "/" "\\\\" CurrentBinaryDir "${CMAKE_CURRENT_BINARY_DIR}")
string(REGEX REPLACE "/" "\\\\" ProjectSourceDir "${PROJECT_SOURCE_DIR}")

if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
    if(WIN64_GENERATOR STREQUAL "Win64")
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSServerd.lib" "${ProjectSourceDir}\\x64")
        add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSServerd.dll" "${ProjectSourceDir}\\x64")
	else()
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSServerd.lib" "${ProjectSourceDir}\\x86")
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSServerd.dll" "${ProjectSourceDir}\\x86")
	endif()
else()
    if(WIN64_GENERATOR STREQUAL "Win64")
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSServer.lib" "${ProjectSourceDir}\\x64")
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSServer.dll" "${ProjectSourceDir}\\x64")
	else()
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSServer.lib" "${ProjectSourceDir}\\x86")
		add_custom_command(TARGET UVSSServer POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSServer.dll" "${ProjectSourceDir}\\x86")
	endif()
endif()
