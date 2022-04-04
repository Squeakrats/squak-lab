cmake_minimum_required(VERSION 3.8)
project(sgrammar)

add_executable(sgrammar src/main.cpp)
target_link_libraries(sgrammar compiler)