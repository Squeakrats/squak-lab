cmake_minimum_required(VERSION 3.8)
project(utility)

add_library(utility
	src/utility.h
	src/utility.cpp
)
target_include_directories(utility PUBLIC src/)