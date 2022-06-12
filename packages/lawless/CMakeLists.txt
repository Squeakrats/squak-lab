cmake_minimum_required(VERSION 3.8)
project(lawless)

add_executable(lawless 
    src/Player.h
    src/PlayerCamera.h
    src/main.cpp
)
target_link_libraries(lawless gl gltf core engine)
set_target_properties(lawless PROPERTIES FOLDER app)

if (EMSCRIPTEN)
    set_target_properties(lawless PROPERTIES LINK_FLAGS "-s MIN_WEBGL_VERSION=2 --embed-file ..\\..\\..\\..\\assets@/ -s TOTAL_MEMORY=51118080")
else()
    target_link_libraries(lawless opengl32)
endif()
