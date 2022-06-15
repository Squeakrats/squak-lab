cmake_minimum_required(VERSION 3.8)
project(net)

add_library(net
	include/squak/net.h
	include/squak/net/tcp/Socket.h
	src/tcp/Socket.cpp
	src/net.cpp
)
target_include_directories(net PRIVATE src/ PUBLIC include/)
target_link_libraries(net utility core ws2_32)