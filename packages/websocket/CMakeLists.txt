add_library(websocket
	include/squak/websocket/Socket.h
	include/squak/websocket/Server.h
	src/Socket.cpp
	src/Server.cpp
)
target_include_directories(websocket PRIVATE src/ PUBLIC include/)
target_link_libraries(websocket utility core net http compression)