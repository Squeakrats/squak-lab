cmake_minimum_required(VERSION 3.8)
project(engine)

add_library(engine
	src/Actor.h
	src/Actor.cpp
	src/Engine.h
	src/Engine.cpp
	src/GLBLoader.h
	src/GLBLoader.cpp
	src/JSONLoader.h
	src/JSONLoader.cpp
)
target_include_directories(engine PUBLIC src/)
target_link_libraries(engine graphics core utility json gltf)