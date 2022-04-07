cmake_minimum_required(VERSION 3.8)
project(compiler)

set(SOURCE
	src/DFA.cpp
	src/DFA.h
	src/NFA.cpp
	src/NFA.h
	src/TokenStream.h
)

add_library(compiler ${SOURCE})
target_include_directories(compiler PUBLIC src/)

add_executable(CompilerTests src/tests/CompilerTests.cpp)
target_link_libraries(CompilerTests gtest_main compiler)