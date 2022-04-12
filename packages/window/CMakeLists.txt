cmake_minimum_required(VERSION 3.8)
project(window)

add_library(window
	src/Window.h
	src/Window.cpp
)
target_include_directories(window PUBLIC src/)