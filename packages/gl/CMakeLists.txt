cmake_minimum_required(VERSION 3.8)
project(gl)

add_library(gl
	include/gl/glutils.h
	src/shaders.h
	src/shaders.cpp
	src/glutils.cpp
	src/Program.h
	src/Renderer.h
	src/Renderer.cpp
	src/RenderingContext.h
	src/RenderingContext.cpp
)
target_include_directories(gl PRIVATE src/ PUBLIC include/)
target_link_libraries(gl graphics utility png glr)