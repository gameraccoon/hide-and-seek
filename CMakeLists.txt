cmake_minimum_required(VERSION 2.8)

set(APP_NAME StealthGamePrototype)
project(${APP_NAME})

# parameters
option(DEBUG_MODE "Debug?" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# constants
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SOURCE_ROOT ${CMAKE_SOURCE_DIR}/src)

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

if (!WIN32)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
endif()

include_directories(include)

include_directories(
    ${CMAKE_SOURCE_DIR}/external/include

    ${SOURCE_ROOT}/DebugHelpers/include
    ${SOURCE_ROOT}/Engine/include
    ${SOURCE_ROOT}/SystemInterface/include
    ${SOURCE_ROOT}/SqliteInterface/include
    ${SOURCE_ROOT}/StealthGame/include
    ${SOURCE_ROOT}/Editor/include

    ${CMAKE_SOURCE_DIR}/MainApp
)

link_directories(
    /usr/local/lib
    ${CMAKE_SOURCE_DIR}/external/lib
    ${CMAKE_SOURCE_DIR}/vs_project
)

function(add_src_folder local_path global_path)
    file(GLOB src "${global_path}/*.h")
    aux_source_directory(${global_path} src)
    if(WIN32)
        string(REPLACE "/" "\\" _group_path "${local_path}")
        source_group(${_group_path} FILES ${src})
    else()
        source_group(${local_path} FILES ${src})
    endif()
    set(TEMP_SRC1 ${TEMP_SRC1} ${src} PARENT_SCOPE)
endfunction()

function(recursive_add_src_folders root_path folder_path)
    set(TEMP_SRC1 "")
    add_src_folder(${folder_path} "${root_path}/${folder_path}")
    file(GLOB folders RELATIVE "${root_path}" "${root_path}/${folder_path}/*")
    foreach(folder ${folders})
        recursive_add_src_folders(${root_path} ${folder})
    endforeach()
    set(TEMP_SRC ${TEMP_SRC} ${TEMP_SRC1} PARENT_SCOPE)
endfunction()

function(add_folder_as_library folder_path name)
    set(TEMP_SRC "")

    recursive_add_src_folders(${folder_path} src)
    recursive_add_src_folders(${folder_path} include)

    add_library(${name} ${TEMP_SRC})
endfunction()

add_folder_as_library(${SOURCE_ROOT}/DebugHelpers DebugHelpersLib)
add_folder_as_library(${SOURCE_ROOT}/Engine EngineLib)
add_folder_as_library(${SOURCE_ROOT}/SystemInterface SystemInterfaceLib)
add_folder_as_library(${SOURCE_ROOT}/SqliteInterface SqliteInterfaceLib)
add_folder_as_library(${SOURCE_ROOT}/StealthGame StealthGameLib)

#add_folder_as_library(${SOURCE_ROOT}/Editor)

if (NOT ${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY "${PROJECT_SOURCE_DIR}/resources" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_executable(${APP_NAME} src/main/win32/main.cpp)
set(GENERAL_PROJECT_LIBS
#    sqlite3
#    liblua52

    StealthGameLib
    SqliteInterfaceLib
    SystemInterfaceLib
    EngineLib
    DebugHelpersLib
)

if (WIN32)
    set(GENERAL_PROJECT_LIBS
        ${GENERAL_PROJECT_LIBS}
        glew32
        glew32s
        SDL2
        SDL2_image
        SDL2main
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
install(TARGETS ${APP_NAME} RUNTIME DESTINATION ${BIN_DIR})

set(TESTS_NAME UnitTests)
set(TESTS_SRC
    win32/tests_main.cpp
    external/googletest/src/gtest-all.cc
)
aux_source_directory(src/UnitTests TESTS_SRC)
include_directories(external/googletest)
add_executable(${TESTS_NAME} ${TESTS_SRC})
target_link_libraries(${TESTS_NAME}
    ${GENERAL_PROJECT_LIBS}
)
