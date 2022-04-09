cmake_minimum_required(VERSION 3.8)
project(json)

add_library(json
	src/AST.h
	src/parser.h
	src/parser.cpp
	src/json.h
	src/json.cpp
	src/json.sg
)
target_include_directories(json PUBLIC src/)
target_link_libraries(json compiler RegularExpression)

add_executable(JsonTests src/tests/JsonTests.cpp)
target_link_libraries(JsonTests gtest_main json)
set_target_properties(JsonTests PROPERTIES FOLDER test)