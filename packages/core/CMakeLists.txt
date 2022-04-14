cmake_minimum_required(VERSION 3.8)
project(core)

add_library(core
	src/Engine.h
	src/Engine.cpp
)
target_include_directories(core PUBLIC src/)
target_link_libraries(core graphics)