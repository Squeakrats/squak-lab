cmake_minimum_required(VERSION 3.8)
project(compression)

add_library(compression
	include/squak/inflate.h
	src/BitStream.h
	src/inflate.cpp
)
target_include_directories(compression PRIVATE src/ PUBLIC include/)
target_link_libraries(compression utility core)

add_executable(CompressionTests src/tests/CompressionTests.cpp)
target_link_libraries(CompressionTests gtest_main compression)
set_target_properties(CompressionTests PROPERTIES FOLDER test)