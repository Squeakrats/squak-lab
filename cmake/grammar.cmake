function(grammar source)
add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.h
		${CMAKE_CURRENT_BINARY_DIR}/generated/Parser.generated.cpp
    COMMAND grammar
             ${CMAKE_CURRENT_SOURCE_DIR}/${source}
             ${CMAKE_CURRENT_BINARY_DIR}/generated/
    DEPENDS grammar ${source}
)
endfunction()