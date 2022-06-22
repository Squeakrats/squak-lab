set(SOURCES
	include/squak/net.h
	include/squak/net/tcp/Socket.h
	src/net.cpp)

if (NOT EMSCRIPTEN)
list(APPEND SOURCES 
	include/squak/net/Poller.h
	src/tcp/Socket.cpp
	src/Poller.cpp)
endif()

add_library(net ${SOURCES})
target_include_directories(net PRIVATE src/ PUBLIC include/)
target_link_libraries(net utility core)
if (NOT EMSCRIPTEN)
	target_link_libraries(net ws2_32)
endif()