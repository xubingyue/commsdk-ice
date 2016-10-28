#-DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013"
#-DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013"
#-DCMAKE_CONFIGURATION_TYPES:STRING=Debug -G "Visual Studio 12 2013 Win64"
#-DCMAKE_CONFIGURATION_TYPES:STRING=Release -G "Visual Studio 12 2013 Win64"

cmake_minimum_required(VERSION 2.6)

project(client)

string(REGEX MATCH "Win64" WIN64_GENERATOR ${CMAKE_GENERATOR})

set(UVSS_COMM_SDK_VER "sdk")

configure_file(
		"${PROJECT_SOURCE_DIR}/version.h.in"
		"${PROJECT_BINARY_DIR}/version.h")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "C:/ZeroC/Ice-3.6.3/include")

if(WIN64_GENERATOR STREQUAL "Win64")
	link_directories("C:/ZeroC/Ice-3.6.3/lib/x64")
else()
	link_directories("C:/ZeroC/Ice-3.6.3/lib")
endif()

add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/clientserver.h ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp
		COMMAND COMMAND slice2cpp -IC:/ZeroC/Ice-3.6.3/slice ${CMAKE_CURRENT_SOURCE_DIR}/../clientserver.ice
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../clientserver.ice)

set(HEADERS ${CMAKE_CURRENT_BINARY_DIR}/clientserver.h clienti.h uvssclient.h uvssclientsdk.h)
set(SOURCES ${CMAKE_CURRENT_BINARY_DIR}/clientserver.cpp clienti.cpp uvssclient.cpp uvssclientsdk.cpp)

if(NOT CMAKE_DEBUG_POSTFIX)
	set(CMAKE_DEBUG_POSTFIX d)
endif()

add_library(UVSSSDK SHARED ${HEADERS} ${SOURCES})

target_link_libraries(UVSSSDK debug "iced.lib" optimized "ice.lib")
target_link_libraries(UVSSSDK debug "iceutild.lib" optimized "iceutil.lib")

string(REGEX REPLACE "/" "\\\\" CurrentBinaryDir "${CMAKE_CURRENT_BINARY_DIR}")
string(REGEX REPLACE "/" "\\\\" ProjectSourceDir "${PROJECT_SOURCE_DIR}")

if(CMAKE_CONFIGURATION_TYPES STREQUAL "Debug")
    if(WIN64_GENERATOR STREQUAL "Win64")
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSSDKd.lib" "${ProjectSourceDir}\\x64")
        add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSSDKd.dll" "${ProjectSourceDir}\\x64")
	else()
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSSDKd.lib" "${ProjectSourceDir}\\x86")
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Debug\\UVSSSDKd.dll" "${ProjectSourceDir}\\x86")
	endif()
else()
    if(WIN64_GENERATOR STREQUAL "Win64")
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSSDK.lib" "${ProjectSourceDir}\\x64")
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSSDK.dll" "${ProjectSourceDir}\\x64")
	else()
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSSDK.lib" "${ProjectSourceDir}\\x86")
		add_custom_command(TARGET UVSSSDK POST_BUILD COMMAND copy "${CurrentBinaryDir}\\Release\\UVSSSDK.dll" "${ProjectSourceDir}\\x86")
	endif()
endif()
