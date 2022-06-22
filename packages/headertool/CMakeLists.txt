project(headertool)

add_executable(headertool
    src/main.cpp
)
target_link_libraries(headertool core)
set_target_properties(headertool PROPERTIES FOLDER tool)
