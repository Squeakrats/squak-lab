cmake_minimum_required(VERSION 3.8)
project(glr)

add_executable(glr
	src/main.cpp
)
target_link_libraries(glr xml)