cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(APP_NAME GameMain)
project(${APP_NAME})

set(EXECUTABLE_NAME Game)

# parameters
option(DEBUG_MODE "Debug?" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# constants
set(GAME_BASE_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GAME_BASE_DIR}/cmake")
set(GAME_BIN_DIR ${GAME_BASE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAME_BIN_DIR})
set(GAME_SRC_DIR ${GAME_BASE_DIR}/src)
set(EXTERNALS_DIR ${GAME_BASE_DIR}/external)
set(INTERMEDIATES_DIR ${GAME_BASE_DIR}/build/game)

file (STRINGS "${GAME_BASE_DIR}/VersionInfo" EXECUTABLE_VERSION)
file(STRINGS "${GAME_BASE_DIR}/BuildNum" BUILD_NUMBER)
project(${APP_NAME} VERSION ${EXECUTABLE_VERSION})

file(MAKE_DIRECTORY ${GAME_BIN_DIR})

# definitions
if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
	add_definitions(-D_DEBUG)
	add_definitions(-DDEBUG)
else(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

add_definitions(-DGLEW_NO_GLU)
add_definitions(-DGLM_FORCE_RADIANS)

# compiler parameters
if (MSVC)
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
	endif()
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -std=c++17")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

if (NOT WIN32)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    include_directories(${SDL2_INCLUDE_DIR})
endif()

include_directories(include)

include_directories(
	${EXTERNALS_DIR}/include
	${GAME_SRC_DIR}
)

link_directories(
	${EXTERNALS_DIR}/lib
	${GAME_BASE_DIR}/vs_project
)

function(add_folder_as_library folder_path name)
	file(GLOB_RECURSE TEMP_SRC RELATIVE "" FOLLOW_SYMLINKS "${folder_path}/*")
	add_library(${name} ${TEMP_SRC})
endfunction()

add_folder_as_library(${GAME_SRC_DIR}/Debug Debug)
add_folder_as_library(${GAME_SRC_DIR}/ECS Ecs)
add_folder_as_library(${GAME_SRC_DIR}/GameData GameData)
add_folder_as_library(${GAME_SRC_DIR}/GameLogic GameLogic)
add_folder_as_library(${GAME_SRC_DIR}/Utils Utils)
add_folder_as_library(${GAME_SRC_DIR}/HAL Hal)

file(GLOB_RECURSE APP_SRC RELATIVE "" FOLLOW_SYMLINKS "${GAME_SRC_DIR}/GameMain/*")
add_executable(${APP_NAME} "${APP_SRC}")

# Detour
option(RECASTNAVIGATION_STATIC "Build static libraries for detour" ON)
add_subdirectory(${EXTERNALS_DIR}/sources/recastnavigation/Detour
	${INTERMEDIATES_DIR}/external/Detour
	EXCLUDE_FROM_ALL)
link_directories(${INTERMEDIATES_DIR}/external/Detour)
add_dependencies(${APP_NAME} Detour)
include_directories(${EXTERNALS_DIR}/sources/recastnavigation/Detour/Include)

# Polypartition
add_library(Polypartition EXCLUDE_FROM_ALL ${EXTERNALS_DIR}/sources/polypartition/src/polypartition.cpp)
add_dependencies(${APP_NAME} Polypartition)
include_directories(${EXTERNALS_DIR}/sources/polypartition/include)

set(GENERAL_PROJECT_LIBS
#	liblua52

    GameLogic
	Hal
	Utils
	GameData
	Ecs
	Debug
	Detour
	Polypartition

	SDL2
	SDL2_image
	SDL2main
)

if (WIN32)
	set(GENERAL_PROJECT_LIBS
		${GENERAL_PROJECT_LIBS}
		glew32
		glew32s
		SDL2test
		opengl32
	)
else()
	set(GENERAL_PROJECT_LIBS
		${GENERAL_PROJECT_LIBS}
		${SDL2_LIBRARY}
		${SDL2_IMAGE_LIBRARY}
		GL
		stdc++fs
	)
endif()

target_link_libraries(${APP_NAME}
	${GENERAL_PROJECT_LIBS}
)

set_target_properties(${APP_NAME} PROPERTIES
	VERSION "${EXECUTABLE_VERSION}.${BUILD_NUMBER}"
	SOVERSION ${BUILD_NUMBER}
	EXPORT_NAME ${EXECUTABLE_NAME}
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${GAME_BIN_DIR}"
)

# Unit tests
set(UNITTESTS_NAME UnitTests)
file(GLOB_RECURSE UNITTESTS_SRC RELATIVE "" FOLLOW_SYMLINKS "${GAME_SRC_DIR}/UnitTests/*")
set(UNITTESTS_SRC
	${UNITTESTS_SRC}
	${EXTERNALS_DIR}/sources/googletest/src/gtest-all.cc
)
include_directories(${EXTERNALS_DIR}/sources/googletest)
add_executable(${UNITTESTS_NAME} ${UNITTESTS_SRC})
target_link_libraries(${UNITTESTS_NAME}
	${GENERAL_PROJECT_LIBS}
)

# Autotests
set(AUTOTESTS_NAME AutoTests)
file(GLOB_RECURSE AUTOTESTS_SRC RELATIVE "" FOLLOW_SYMLINKS "${GAME_SRC_DIR}/AutoTests/*")
add_executable(${AUTOTESTS_NAME} ${AUTOTESTS_SRC})
target_link_libraries(${AUTOTESTS_NAME}
	${GENERAL_PROJECT_LIBS}
)

# copy resources
if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
	file(REMOVE_RECURSE ${GAME_BIN_DIR}/resources)
	file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${GAME_BIN_DIR})
endif()

# code generation
find_package(PythonInterp 3.5 REQUIRED)
add_custom_target(
	CodeGeneration
	COMMAND ${PYTHON_EXECUTABLE} ${GAME_BASE_DIR}/scripts/build/cpp_code_generate.py ${GAME_BASE_DIR}
	COMMENT "C++ code generation"
)
add_dependencies(GameData CodeGeneration)
add_dependencies(Utils CodeGeneration)
add_dependencies(${APP_NAME} CodeGeneration)
