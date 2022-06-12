cmake_minimum_required(VERSION 3.8)
project(engine)

add_library(engine
	include/squak/engine/Actor.h
	include/squak/engine/Engine.h
	include/squak/engine/GLBLoader.h
	include/squak/engine/JSONLoader.h
	src/Actor.cpp
	src/Engine.cpp
	src/GLBLoader.cpp
	src/JSONLoader.cpp
)
target_include_directories(engine PRIVATE src/ PUBLIC include/)
target_link_libraries(engine graphics core utility json gltf window)