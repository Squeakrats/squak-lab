cmake_minimum_required(VERSION 3.8)
project(sgrammar)

add_executable(sgrammar
    src/main.cpp 
    src/Grammar.h
    src/Grammar.cpp
    src/Parser.cpp
    src/Parser.h
    src/Tokenizer.cpp
    src/Tokenizer.h
)
target_link_libraries(sgrammar compiler)