cmake_minimum_required(VERSION 3.8)
project(math)

add_library(math
	src/Vector3.h
	src/Vector3.cpp
)
target_include_directories(math PUBLIC src/)

add_executable(MathTests src/tests/MathTests.cpp)
target_link_libraries(MathTests gtest_main math)
set_target_properties(MathTests PROPERTIES FOLDER test)