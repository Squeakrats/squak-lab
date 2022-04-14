cmake_minimum_required(VERSION 3.8)
project(gl)

add_library(gl
	include/gl/Renderer.h
	include/gl/glutils.h
	src/shaders.h
	src/shaders.cpp
	src/glutils.cpp
	src/Renderer.cpp
)
target_include_directories(gl PRIVATE src/ PUBLIC include/)
target_link_libraries(gl graphics utility)