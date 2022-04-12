cmake_minimum_required(VERSION 3.8)
project(gltf)

add_library(gltf
    src/gltf.h
    src/gltf.cpp
)
target_include_directories(gltf PUBLIC src/)
target_link_libraries(gltf json graphics)

add_executable(GLTFTests src/tests/GLTFTests.cpp)
target_link_libraries(GLTFTests gtest_main gltf)
set_target_properties(GLTFTests PROPERTIES FOLDER test)