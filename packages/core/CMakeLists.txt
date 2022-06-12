cmake_minimum_required(VERSION 3.8)
project(core)

add_library(core
	include/squak/core/AssetManager.h
	include/squak/core/Buffer.h
	include/squak/core/BufferView.h
	src/AssetManager.cpp
)
target_include_directories(core PRIVATE src/ PUBLIC include/)