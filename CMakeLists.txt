cmake_minimum_required(VERSION 3.8)
project(squak-lab)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(packages/compiler)
add_subdirectory(packages/lawless)
add_subdirectory(packages/sgrammar)