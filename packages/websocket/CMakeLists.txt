set(SOURCES
	include/squak/websocket/Socket.h
	include/squak/websocket/Server.h)

if (NOT EMSCRIPTEN)
list(APPEND SOURCES
	src/Socket.cpp
	src/Server.cpp)
endif()

add_library(websocket ${SOURCES})
target_include_directories(websocket PRIVATE src/ PUBLIC include/)
target_link_libraries(websocket utility core net http compression)