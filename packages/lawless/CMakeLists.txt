cmake_minimum_required(VERSION 3.8)
project(lawless)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/generated/PlayerCamera.generated.h
    COMMAND headertool
             ${CMAKE_CURRENT_SOURCE_DIR}/src/PlayerCamera.h
             ${CMAKE_CURRENT_BINARY_DIR}/generated/PlayerCamera.generated.h
    DEPENDS headertool src/PlayerCamera.h
)

add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/generated/Player.generated.h
    COMMAND headertool
             ${CMAKE_CURRENT_SOURCE_DIR}/src/Player.h
             ${CMAKE_CURRENT_BINARY_DIR}/generated/Player.generated.h
    DEPENDS headertool src/Player.h
)

add_executable(lawless 
    src/Player.h
    src/PlayerCamera.h
    src/main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/generated/PlayerCamera.generated.h
    ${CMAKE_CURRENT_BINARY_DIR}/generated/Player.generated.h
)
target_link_libraries(lawless gl gltf core engine websocket)
set_target_properties(lawless PROPERTIES FOLDER app)
target_include_directories(lawless PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated)

if (EMSCRIPTEN)
    set_target_properties(lawless PROPERTIES LINK_FLAGS "-s MIN_WEBGL_VERSION=2 --embed-file ..\\..\\..\\..\\assets@/ -s TOTAL_MEMORY=51118080")
else()
    target_link_libraries(lawless opengl32)
endif()
