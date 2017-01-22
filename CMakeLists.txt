cmake_minimum_required(VERSION 2.8)

set(APP_NAME Game)
project(${APP_NAME})

option(DEBUG_MODE "Debug or release?" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE DEBUG)
	add_definitions(-D_DEBUG)
else(DEBUG_MODE)
	set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

if(MSVC)
	#set(CMAKE_C_FLAGS_DEBUG "-DCOCOS2D_DEBUG=1")
	#set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})

	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS")

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
	${SOURCE_ROOT}/EngineInterface/include
	${SOURCE_ROOT}/SqliteInterface/include
	${SOURCE_ROOT}/StealthGame/include
	${SOURCE_ROOT}/MainApp
	${SOURCE_ROOT}/Editor

	${CMAKE_SOURCE_DIR}/MainApp
)
link_directories(
	#/usr/local/lib
	${CMAKE_SOURCE_DIR}/external/lib
	${CMAKE_SOURCE_DIR}/vs_project/src/DebugHelpers/Debug
	${CMAKE_SOURCE_DIR}/vs_project/src/Engine/Debug
	${CMAKE_SOURCE_DIR}/vs_project/src/EngineInterface/Debug
	${CMAKE_SOURCE_DIR}/vs_project/src/MainApp/Debug
	${CMAKE_SOURCE_DIR}/vs_project/src/SqliteInterface/Debug
	${CMAKE_SOURCE_DIR}/vs_project/src/StealthGame/Debug
)

elseif(UNIX) #assumed linux
include_directories(
	#...
)

link_directories(
	#...
)
endif()

add_subdirectory(${SOURCE_ROOT}/DebugHelpers)
add_subdirectory(${SOURCE_ROOT}/Engine)
add_subdirectory(${SOURCE_ROOT}/EngineInterface)
add_subdirectory(${SOURCE_ROOT}/SqliteInterface)
add_subdirectory(${SOURCE_ROOT}/StealthGame)

add_subdirectory(${SOURCE_ROOT}/MainApp)
#add_subdirectory(${SOURCE_ROOT}/Editor)

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

	DebugHelpersLib
	EngineLib
	EngineInterfaceLib
	SqliteInterfaceLib
	StealthGameLib
	MainAppLib
)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}"
)

# if (WIN32)
# 	#also copying dlls to binary directory for the executable to run
# 	pre_build(${APP_NAME}
# 		COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${APP_BIN_DIR}/Resources
# 		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
# 		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/external/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
# 	)
# else()
# 	pre_build(${APP_NAME}
# 		COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
# 		COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
# 	)
# endif()
