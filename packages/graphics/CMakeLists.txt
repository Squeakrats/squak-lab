cmake_minimum_required(VERSION 3.8)
project(graphics)

add_library(graphics
	src/SceneNode.h
	src/SceneNode.cpp
	src/Geometry.h
	src/Geometry.cpp
)
target_include_directories(graphics PUBLIC src/)
target_link_libraries(graphics math)