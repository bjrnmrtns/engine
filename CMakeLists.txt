cmake_minimum_required(VERSION 2.6)
project(demo)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native")

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

add_executable(demo
	thirdparty/glad/src/glad.c
	camera.h
	font.h
	font.cpp
	main.cpp
    rendersource.cpp
    rendersource.h
	shaderprogram.cpp
	shaderprogram.h
    ui.cpp
    ui.h
)

target_include_directories(demo PUBLIC
	${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} thirdparty/glad/include
)

target_link_libraries(demo
	${SDL2_LIBRARIES}
	${FREETYPE_LIBRARIES}
	dl
)
