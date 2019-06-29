cmake_minimum_required(VERSION 2.8)

set(APP_NAME Game)
project(${APP_NAME})

# parameters
option(DEBUG_MODE "Debug?" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# constants
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(SOURCE_ROOT ${CMAKE_SOURCE_DIR}/src)
set(EXTERNALS_DIR ${CMAKE_SOURCE_DIR}/external)
set(INTERMEDIATES_DIR ${CMAKE_SOURCE_DIR}/build/game)

file(MAKE_DIRECTORY ${BIN_DIR})

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
	${SOURCE_ROOT}
)

link_directories(
    ${EXTERNALS_DIR}/lib
    ${CMAKE_SOURCE_DIR}/vs_project
)

function(add_folder_as_library folder_path name)
	file(GLOB_RECURSE TEMP_SRC RELATIVE "" FOLLOW_SYMLINKS "${folder_path}/*")
    add_library(${name} ${TEMP_SRC})
endfunction()

add_folder_as_library(${SOURCE_ROOT}/Debug Debug)
add_folder_as_library(${SOURCE_ROOT}/ECS Ecs)
add_folder_as_library(${SOURCE_ROOT}/GameData GameData)
add_folder_as_library(${SOURCE_ROOT}/Utils Utils)
add_folder_as_library(${SOURCE_ROOT}/HAL Hal)

file(GLOB_RECURSE APP_SRC RELATIVE "" FOLLOW_SYMLINKS "${SOURCE_ROOT}/Game/*")
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
#    liblua52

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

# Unit tests
set(TESTS_NAME UnitTests)
set(TESTS_SRC
	${SOURCE_ROOT}/UnitTests/tests_main.cpp
    ${EXTERNALS_DIR}/sources/googletest/src/gtest-all.cc
)
aux_source_directory(${SOURCE_ROOT}/UnitTests/Tests TESTS_SRC)
include_directories(${EXTERNALS_DIR}/sources/googletest)
add_executable(${TESTS_NAME} ${TESTS_SRC})
target_link_libraries(${TESTS_NAME}
    ${GENERAL_PROJECT_LIBS}
)

# copy resources
if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    file(REMOVE_RECURSE ${BIN_DIR}/resources)
    file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${BIN_DIR})
endif()

# code generation
find_package(PythonInterp 3.5 REQUIRED)
add_custom_target(
    CodeGeneration
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/build/cpp_code_generate.py ${CMAKE_SOURCE_DIR}
    COMMENT "C++ code generation"
)
add_dependencies(GameData CodeGeneration)
add_dependencies(Utils CodeGeneration)
add_dependencies(${APP_NAME} CodeGeneration)
