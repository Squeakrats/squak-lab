cmake_minimum_required(VERSION 3.8)
project(gl)

add_library(gl
	src/Renderer.h
	src/Renderer.cpp
)
target_include_directories(gl PUBLIC src/)
target_link_libraries(gl graphics)