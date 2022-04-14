cmake_minimum_required(VERSION 3.8)
project(engine)

add_library(engine
	src/Engine.h
	src/Engine.cpp
)
target_include_directories(engine PUBLIC src/)
target_link_libraries(engine graphics core)