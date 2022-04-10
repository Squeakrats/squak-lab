cmake_minimum_required(VERSION 3.8)
project(json)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
    COMMAND grammar
             ${CMAKE_CURRENT_SOURCE_DIR}/src/json.g
             ${CMAKE_CURRENT_BINARY_DIR}/generated/
    DEPENDS grammar src/json.g
)

add_library(json
	src/AST.h
	src/json.h
	src/json.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
)
target_include_directories(json
	PUBLIC
		src/
	PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
)
target_link_libraries(json compiler RegularExpression)

add_executable(JsonTests src/tests/JsonTests.cpp)
target_link_libraries(JsonTests gtest_main json)
set_target_properties(JsonTests PROPERTIES FOLDER test)