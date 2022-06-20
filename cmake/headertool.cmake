function(headertool source destination)
add_custom_command(
    OUTPUT
		${CMAKE_CURRENT_BINARY_DIR}/${destination}
    COMMAND headertool
             ${CMAKE_CURRENT_SOURCE_DIR}/${source}
             ${CMAKE_CURRENT_BINARY_DIR}/${destination}
    DEPENDS headertool ${source}
)
endfunction()