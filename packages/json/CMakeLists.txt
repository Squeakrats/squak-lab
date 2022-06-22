include(grammar)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
grammar(src/json.g)

add_library(json
	include/squak/json.h
	src/AST.h
	src/json.cpp
	src/json.g
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
)
target_include_directories(json
	PUBLIC
		include/
	PRIVATE 
		src/
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
)
target_link_libraries(json regex)

if (NOT EMSCRIPTEN)
	add_executable(JsonTests src/tests/JsonTests.cpp)
	target_link_libraries(JsonTests gtest_main json)
	set_target_properties(JsonTests PROPERTIES FOLDER test)
endif()