cmake_minimum_required(VERSION 3.8)
project(xml)
include(grammar)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
grammar(src/xml.g)

add_library(xml
	src/ast.h
	src/xml.h
	src/xml.cpp
	src/xml.g
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
)
target_include_directories(xml
	PUBLIC
		src/
	PRIVATE 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
)
target_link_libraries(xml regex)

if (NOT EMSCRIPTEN)
	add_executable(XmlTests src/tests/XmlTests.cpp)
	target_link_libraries(XmlTests gtest_main xml)
	set_target_properties(XmlTests PROPERTIES FOLDER test)
endif()