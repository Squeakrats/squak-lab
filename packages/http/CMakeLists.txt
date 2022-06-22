set(SOURCES 
	include/squak/http.h
	include/squak/http/Server.h)

if(NOT EMSCRIPTEN)
list(APPEND SOURCES
	src/http.cpp
	src/Server.cpp)
endif()

add_library(http ${SOURCES})
target_include_directories(http PRIVATE src/ PUBLIC include/)
target_link_libraries(http utility core net)