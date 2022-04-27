cmake_minimum_required(VERSION 3.8)
project(png)

add_library(png
	src/png.h
	src/png.cpp
	src/ByteStream.h
)
target_include_directories(png PUBLIC src/)
target_link_libraries(png compression core)