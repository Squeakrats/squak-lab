cmake_minimum_required(VERSION 3.8)
project(RegularExpression)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
    COMMAND grammar
             ${CMAKE_CURRENT_SOURCE_DIR}/src/RegularExpression.g
             ${CMAKE_CURRENT_BINARY_DIR}/generated/
    DEPENDS grammar src/RegularExpression.g
)

add_library(RegularExpression
	src/AST.h
	src/RegularExpression.h
	src/RegularExpression.cpp
	src/Tokenizer.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
)
target_include_directories(RegularExpression
	PUBLIC
		src/
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
)
target_link_libraries(RegularExpression compiler utility)

add_executable(RegularExpressionTests src/tests/RegularExpressionTests.cpp)
target_link_libraries(RegularExpressionTests gtest_main RegularExpression)
set_target_properties(RegularExpressionTests PROPERTIES FOLDER test)