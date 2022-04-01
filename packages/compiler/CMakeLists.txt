cmake_minimum_required(VERSION 3.8)
project(compiler)

set(SOURCE
	src/BackusNaur/Grammar.h
	src/BackusNaur/Grammar.cpp
	src/BackusNaur/Parser.cpp
	src/BackusNaur/Parser.h
	src/BackusNaur/Tokenizer.cpp
	src/BackusNaur/Tokenizer.h
	src/DFA.cpp
	src/DFA.h
	src/NFA.cpp
	src/NFA.h
	src/RegularExpression/Tokenizer.cpp
	src/RegularExpression/Tokenizer.h
)

add_library(compiler ${SOURCE})
target_include_directories(compiler PUBLIC src/)

enable_testing()

add_executable(CompilerTests
	src/BackusNaur/tests/BackusNaurTests.cpp
	src/RegularExpression/tests/RegularExpressionTests.cpp
	src/tests/CompilerTests.cpp
)

# Follow google test framework setup instructions
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(
  CompilerTests
  gtest_main
  compiler
)

include(GoogleTest)
gtest_discover_tests(CompilerTests)
