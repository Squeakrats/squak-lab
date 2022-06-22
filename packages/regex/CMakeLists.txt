include(grammar)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
grammar(src/regex.g)

add_library(regex
	src/AST.h
	src/regex.h
	src/regex.cpp
	src/regex.g
	src/DFA.h
	src/DFA.cpp
	src/NFA.h
	src/NFA.cpp
	src/Tokenizer.cpp
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
	${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
)
target_include_directories(regex
	PUBLIC
		src/
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
)
target_link_libraries(regex utility core)

if (NOT EMSCRIPTEN)
	add_executable(RegularExpressionTests src/tests/RegularExpressionTests.cpp)
	target_link_libraries(RegularExpressionTests gtest_main regex)
	set_target_properties(RegularExpressionTests PROPERTIES FOLDER test)
endif()
