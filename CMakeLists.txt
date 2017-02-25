cmake_minimum_required(VERSION 3.7)

project(demo)

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


add_executable(demo
	thirdparty/glad/src/glad.c
	main.cpp
	Configuration.h
	UiElements.cpp
	UiElements.h
	Model.cpp
	Model.h
	Camera.cpp
	Camera.h
	RenderTools.cpp
	RenderTools.h
	RenderMesh.cpp
	RenderMesh.h
	RenderUI.cpp
	RenderUI.h
	VertexDefinitions.h)

target_include_directories(demo PUBLIC
	${GLM_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} thirdparty/glad/include
)

target_link_libraries(demo
	${SDL2_LIBRARIES}
	${FREETYPE_LIBRARIES}
)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
