project(Renderer)

find_package(GLEW REQUIRED)

add_library(${PROJECT_NAME} STATIC
	src/RenderTools.cpp
	include/Renderer/RenderTools.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${GLEW_INCLUDE_DIRS}
    PUBLIC 
        include
)

