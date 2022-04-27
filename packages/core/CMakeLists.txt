cmake_minimum_required(VERSION 3.8)
project(core)

add_library(core
	src/AssetManager.h
	src/AssetManager.cpp
	src/Buffer.h
	src/BufferView.h
)
target_include_directories(core PUBLIC src/)