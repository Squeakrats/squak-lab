cmake_minimum_required(VERSION 3.8)
project(http)

add_library(http
	include/squak/http.h
	src/http.cpp
)
target_include_directories(http PRIVATE src/ PUBLIC include/)
target_link_libraries(http utility core net)