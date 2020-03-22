cmake_minimum_required(VERSION 3.16)
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
set(BIN_PLATFORM "x64")

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
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	endif()
	set(PROJECT_CXX_FLAGS /W4 /std:c++17)
	set(EXTERNALS_CXX_FLAGS /W0 /std:c++17)
	set(EXTERNALS_C_FLAGS /W0)

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} /MDd")
else()
	set(PROJECT_CXX_FLAGS -std=c++17 -Wall -Wextra -pedantic -Werror)
	set(EXTERNALS_CXX_FLAGS -std=c++17)
	set(EXTERNALS_C_FLAGS "")

	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

# uncomment to enable specific debug features
# add_definitions(-DDETECT_MEMORY_LEAKS)
# add_definitions(-DDETECT_DOUBLE_DELETE)

# code generation
execute_process(
	COMMAND ${PYTHON_EXECUTABLE} ${GAME_BASE_DIR}/scripts/build/code_generate.py ${GAME_BASE_DIR}
)

find_package(PythonInterp 3.5 REQUIRED)
if (NOT WIN32)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
endif()

include_directories(include)

include_directories(
	${EXTERNALS_DIR}/include
	${EXTERNALS_DIR}/include/sdl
	${EXTERNALS_DIR}/sources
	${GAME_SRC_DIR}
)

link_directories(
	${EXTERNALS_DIR}/lib/${BIN_PLATFORM}
	${GAME_BASE_DIR}/vs_project
)

function(add_folder_as_library folder_path name)
	file(GLOB_RECURSE TEMP_SRC RELATIVE "" FOLLOW_SYMLINKS "${folder_path}/*")
	add_library(${name} ${TEMP_SRC})
	target_precompile_headers(${name} PRIVATE "${GAME_SRC_DIR}/Base/precomp.h")
	set_target_properties(${name} PROPERTIES UNITY_BUILD ON)
	target_compile_options(${name} PRIVATE ${PROJECT_CXX_FLAGS})
endfunction()

add_folder_as_library(${GAME_SRC_DIR}/Base Base)
add_folder_as_library(${GAME_SRC_DIR}/ECS Ecs)
add_folder_as_library(${GAME_SRC_DIR}/GameData GameData)
add_folder_as_library(${GAME_SRC_DIR}/GameLogic GameLogic)
add_folder_as_library(${GAME_SRC_DIR}/Utils Utils)
add_folder_as_library(${GAME_SRC_DIR}/HAL Hal)

file(GLOB_RECURSE APP_SRC RELATIVE "" FOLLOW_SYMLINKS "${GAME_SRC_DIR}/GameMain/*")
add_executable(${APP_NAME} "${APP_SRC}")
target_compile_options(${APP_NAME} PRIVATE ${PROJECT_CXX_FLAGS})

# Detour
option(RECASTNAVIGATION_STATIC "Build static libraries for detour" ON)
add_subdirectory(${EXTERNALS_DIR}/sources/recastnavigation/Detour
	${INTERMEDIATES_DIR}/external/Detour
	EXCLUDE_FROM_ALL)
link_directories(${INTERMEDIATES_DIR}/external/Detour)
add_dependencies(${APP_NAME} Detour)
include_directories(${EXTERNALS_DIR}/sources/recastnavigation/Detour/Include)
target_compile_options(Detour PRIVATE ${EXTERNALS_CXX_FLAGS})

# Polypartition
add_library(Polypartition EXCLUDE_FROM_ALL ${EXTERNALS_DIR}/sources/polypartition/src/polypartition.cpp)
add_dependencies(${APP_NAME} Polypartition)
include_directories(${EXTERNALS_DIR}/sources/polypartition/include)
target_compile_options(Polypartition PRIVATE ${EXTERNALS_CXX_FLAGS})

set(GENERAL_PROJECT_LIBS
#	liblua52

	GameLogic
	Hal
	Utils
	GameData
	Ecs
	Base

	Detour
	Polypartition

	SDL2
	SDL2_image
	SDL2_mixer
	SDL2_ttf
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
target_compile_options(${UNITTESTS_NAME} PRIVATE ${PROJECT_CXX_FLAGS})
target_link_libraries(${UNITTESTS_NAME}
	${GENERAL_PROJECT_LIBS}
)

# Autotests
set(AUTOTESTS_NAME AutoTests)
file(GLOB_RECURSE AUTOTESTS_SRC RELATIVE "" FOLLOW_SYMLINKS "${GAME_SRC_DIR}/AutoTests/*")
add_executable(${AUTOTESTS_NAME} ${AUTOTESTS_SRC})
target_compile_options(${AUTOTESTS_NAME} PRIVATE ${PROJECT_CXX_FLAGS})
target_link_libraries(${AUTOTESTS_NAME}
	${GENERAL_PROJECT_LIBS}
)

# symlink resources dir
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${PROJECT_SOURCE_DIR}/resources" "${GAME_BIN_DIR}/resources")

if (WIN32)
	# copy dlls
	file(GLOB WIN32_DLLS
		"${EXTERNALS_DIR}/dll/${BIN_PLATFORM}/*"
	)
	file(COPY ${WIN32_DLLS} DESTINATION ${GAME_BIN_DIR})
endif()

# pre-compile additional code generation
add_custom_target(
	AdditionalCodeGeneration
	COMMAND ${PYTHON_EXECUTABLE} ${GAME_BASE_DIR}/scripts/build/precompile_code_generate.py ${GAME_BASE_DIR}
	COMMENT "C++ pre-compile additional code generation"
)
add_dependencies(Base AdditionalCodeGeneration)
