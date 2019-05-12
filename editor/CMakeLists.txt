cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_BUILD_TYPE DEBUG)
add_definitions(-D_DEBUG)
add_definitions(-DDEBUG)
project(editor VERSION "1.0") 
set(REQUIRED_QT_VERSION 5.5.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Gui ${REQUIRED_QT_VERSION} REQUIRED)
find_package(Qt5Widgets ${REQUIRED_QT_VERSION} REQUIRED)
set(editor_LIBS ${editor_LIBS} ${Qt5Core_LIBRARIES})
set(editor_INCLUDE ${editor_INCLUDE} ${Qt5Core_INCLUDE_DIRS})
set(editor_COMPILE_DEFINE ${editor_COMPILE_DEFINE} ${Qt5Core_COMPILE_DEFINITIONS} )
set(editor_LIBS ${editor_LIBS} ${Qt5Gui_LIBRARIES})
set(editor_INCLUDE ${editor_INCLUDE} ${Qt5Gui_INCLUDE_DIRS})
set(editor_COMPILE_DEFINE ${editor_COMPILE_DEFINE} ${Qt5Gui_COMPILE_DEFINITIONS})
set(editor_LIBS ${editor_LIBS} ${Qt5Widgets_LIBRARIES})
set(editor_INCLUDE ${editor_INCLUDE} ${Qt5Widgets_INCLUDE_DIRS})
set(editor_COMPILE_DEFINE ${editor_COMPILE_DEFINE} ${Qt5Widgets_COMPILE_DEFINITIONS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# compiler parameters
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -std=c++17 -lstdc++fs")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

set(editor_SRCS
    src/main.cpp
    src/mainwindow.cpp

    src/mainwindow.ui

    main.qrc
)

aux_source_directory(src/editorcommands editor_SRCS)
aux_source_directory(src/editorcommands/generated editor_SRCS)
aux_source_directory(src/componenteditcontent editor_SRCS)
aux_source_directory(src/componenteditcontent/generated editor_SRCS)
aux_source_directory(src/toolboxes editor_SRCS)

add_subdirectory(Qt-Advanced-Docking-System)

add_executable(editor WIN32 ${editor_SRCS})
if(BUILD_STATIC)
    set(editor_DEFINE ${editor_DEFINE} ADS_STATIC)
endif()

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/DebugHelpers/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/Engine/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src/SqliteInterface/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../external/include"
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

add_folder_as_library(${CMAKE_CURRENT_SOURCE_DIR}/../src/DebugHelpers DebugHelpersLib)
add_folder_as_library(${CMAKE_CURRENT_SOURCE_DIR}/../src/Engine EngineLib)

link_directories(
    /usr/local/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../bin/static-libs
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lib
    ${CMAKE_SOURCE_DIR}/vs_project
)

set(editor_LIBS
    ${editor_LIBS}
#    StealthGameLib
#    SqliteInterfaceLib
#    SystemInterfaceLib
    EngineLib
    DebugHelpersLib
)

if (NOT WIN32)
    set(editor_LIBS
        ${editor_LIBS}
        stdc++fs
    )
endif()

add_dependencies(editor qtadvanceddocking)
target_include_directories(editor PUBLIC
    $<BUILD_INTERFACE:${editor_INCLUDE}>
    $<INSTALL_INTERFACE:include>
    )
target_include_directories(editor PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Qt-Advanced-Docking-System/src" ${editor_INCLUDE})
target_link_libraries(editor PRIVATE qtadvanceddocking ${editor_LIBS})
target_compile_definitions(editor PRIVATE ${editor_DEFINE})
set_target_properties(editor PROPERTIES
    VERSION "1.0"
    SOVERSION 1
    EXPORT_NAME "Stealth Game Editor"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/bin"
)

# code generation
find_package(PythonInterp 3.5 REQUIRED)
add_custom_target(
    CodeGeneration
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/../scripts/build/cpp_code_generate.py ${CMAKE_SOURCE_DIR}/../
    COMMENT "C++ code generation"
)
add_dependencies(EngineLib CodeGeneration)
