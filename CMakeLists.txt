cmake_minimum_required(VERSION 3.7)

project(engine)

find_package(SDL2 REQUIRED)
find_package(OPENGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Configuration.cpp
	src/Configuration.hpp
	src/UiElements.cpp
	src/UiElements.hpp
	src/Model.cpp
	src/Model.hpp
	src/Camera.cpp
	src/Camera.hpp
	src/RenderMesh.cpp
	src/RenderMesh.hpp
	src/RenderUI.cpp
	src/RenderUI.hpp
	src/VertexDefinitions.cpp
	src/VertexDefinitions.hpp
)

add_subdirectory(src/Renderer)

target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
        GLM
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
	${SDL2_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_gl_LIBRARY}
	Renderer
)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
