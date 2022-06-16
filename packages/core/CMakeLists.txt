cmake_minimum_required(VERSION 3.8)
project(core)

add_library(core
	include/squak/core/AssetManager.h
	include/squak/core/Buffer.h
	include/squak/core/BufferView.h
	include/squak/core/IByteStream.h
	include/squak/core/StringStream.h
	include/squak/core/ThreadPool.h
	src/AssetManager.cpp
	src/ThreadPool.cpp
)
target_link_libraries(core utility)
target_include_directories(core PRIVATE src/ PUBLIC include/)