include(headertool)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)
headertool(src/PlayerCamera.h generated/PlayerCamera.generated.h)
headertool(src/Player.h generated/Player.generated.h)

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
    set_target_properties(lawless PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "..\\..\\..\\..\\assets\\")
    target_link_libraries(lawless opengl32)
endif()
