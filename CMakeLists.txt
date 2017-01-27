cmake_minimum_required(VERSION 2.8)

set(APP_NAME Game)
project(${APP_NAME})

option(DEBUG_MODE "Debug or release?" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
	add_definitions(-D_DEBUG)
else(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	#set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wno-reorder -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-unused-variable -Wno-unused-function")

	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

endif()

if(UNIX) #assume linux
set(GAME_SRC
	#proj.linux/main.cpp
)
elseif(WIN32)
set(GAME_SRC
	win32/main.cpp
)
endif()

set(SOURCE_ROOT ${CMAKE_SOURCE_DIR}/src)
if (WIN32)
include_directories(
	${CMAKE_SOURCE_DIR}/external/include

	${SOURCE_ROOT}/DebugHelpers/include
	${SOURCE_ROOT}/Engine/include
	${SOURCE_ROOT}/SystemInterface/include
	${SOURCE_ROOT}/SqliteInterface/include
	${SOURCE_ROOT}/StealthGame/include
	${SOURCE_ROOT}/MainApp/include
	${SOURCE_ROOT}/Editor/include

	${CMAKE_SOURCE_DIR}/MainApp
)
link_directories(
	#/usr/local/lib
	${CMAKE_SOURCE_DIR}/external/lib
	${CMAKE_SOURCE_DIR}/vs_project
)

elseif(UNIX) #assumed linux
include_directories(
	#...
)

link_directories(
	#...
)
endif()

function(add_src_folder local_path global_path)
	file(GLOB src "${global_path}/*.h")
	aux_source_directory(${global_path} src)
	string(REPLACE "/" "\\" _group_path "${local_path}")
	source_group(${_group_path} FILES ${src})
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

add_folder_as_library(${SOURCE_ROOT}/Engine EngineLib)
add_folder_as_library(${SOURCE_ROOT}/DebugHelpers DebugHelpersLib)
add_folder_as_library(${SOURCE_ROOT}/SystemInterface SystemInterfaceLib)
add_folder_as_library(${SOURCE_ROOT}/SqliteInterface SqliteInterfaceLib)
add_folder_as_library(${SOURCE_ROOT}/StealthGame StealthGameLib)

add_folder_as_library(${SOURCE_ROOT}/MainApp MainAppLib)
#add_folder_as_library(${SOURCE_ROOT}/Editor)

if(WIN32)
	# add the executable
	add_executable(${APP_NAME}
		WIN32
		${GAME_SRC}
	)
else()
	# add the executable
	add_executable(${APP_NAME}
		${GAME_SRC}
	)
endif()

target_link_libraries(${APP_NAME}
	glew32
	glew32s
	#liblua52
	SDL2
	SDL2_image
	SDL2main
	SDL2test
	sqlite3

	opengl32

	DebugHelpersLib
	EngineLib
	SystemInterfaceLib
	SqliteInterfaceLib
	StealthGameLib
	MainAppLib
)

set(APP_BIN_DIR "${CMAKE_SOURCE_DIR}/build")

set_target_properties(${APP_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}"
)

if (WIN32)
	#also copying dlls to binary directory for the executable to run
	add_custom_command(TARGET ${APP_NAME}
		PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${APP_BIN_DIR}/$(Configuration)/Resources

		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/zlib1.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/sqlite3.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/SDL2_image.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/SDL2.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/lua52.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/libwebp-4.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/libtiff-5.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/libpng16-16.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/libjpeg-9.dll ${APP_BIN_DIR}/$(Configuration)
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/external/dll/glew32.dll ${APP_BIN_DIR}/$(Configuration)
	)
else()
	# pre_build(${APP_NAME}
	# 	COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
	# 	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
	# )
endif()
