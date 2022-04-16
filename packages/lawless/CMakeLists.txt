cmake_minimum_required(VERSION 3.8)
project(lawless)

add_executable(lawless 
    src/Player.h
    src/Player.cpp
    src/main.cpp
)
target_link_libraries(lawless gl gltf core engine)

if (EMSCRIPTEN)
    set_target_properties(lawless PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s MIN_WEBGL_VERSION=2 --embed-file ..\\..\\..\\..\\assets@/ -s TOTAL_MEMORY=51118080")
endif()
