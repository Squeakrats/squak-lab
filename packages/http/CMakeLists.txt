add_library(http
	include/squak/http.h
	include/squak/http/Server.h
	src/http.cpp
	src/Server.cpp
)
target_include_directories(http PRIVATE src/ PUBLIC include/)
target_link_libraries(http utility core net)