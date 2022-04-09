cmake_minimum_required(VERSION 3.8)
project(RegularExpression)

add_library(RegularExpression
	src/AST.h
	src/RegularExpression.sg
	src/RegularExpression.h
	src/RegularExpression.cpp
	src/Parser.h
	src/Parser.cpp
	src/Tokenizer.cpp
)
target_include_directories(RegularExpression PUBLIC src/)
target_link_libraries(RegularExpression compiler)

add_executable(RegularExpressionTests src/tests/RegularExpressionTests.cpp)
target_link_libraries(RegularExpressionTests gtest_main RegularExpression)
set_target_properties(RegularExpressionTests PROPERTIES FOLDER test)