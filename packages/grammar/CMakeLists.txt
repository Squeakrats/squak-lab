cmake_minimum_required(VERSION 3.8)
project(grammar)

add_executable(grammar
    src/main.cpp 
    src/fragments.h
    src/Grammar.h
    src/Grammar.cpp
    src/Parser.cpp
    src/Parser.h
    src/Tokenizer.cpp
    src/Tokenizer.h
)
target_link_libraries(grammar utility)
set_target_properties(grammar PROPERTIES FOLDER tool)