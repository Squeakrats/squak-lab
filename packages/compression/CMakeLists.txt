cmake_minimum_required(VERSION 3.8)
project(compression)

add_library(compression
	src/BitStream.h
	src/compression.h
	src/compression.cpp
)
target_include_directories(compression PUBLIC src/)
target_link_libraries(compression utility core)

add_executable(CompressionTests src/tests/CompressionTests.cpp)
target_link_libraries(CompressionTests gtest_main compression)
set_target_properties(CompressionTests PROPERTIES FOLDER test)