project(gltf)

add_library(gltf
    include/squak/gltf.h
    src/gltf.cpp
)
target_include_directories(gltf PRIVATE src/ PUBLIC include/)
target_link_libraries(gltf json graphics utility)

if (NOT EMSCRIPTEN)
    add_executable(GLTFTests src/tests/GLTFTests.cpp)
    target_link_libraries(GLTFTests gtest_main gltf)
    set_target_properties(GLTFTests PROPERTIES FOLDER test)
endif()