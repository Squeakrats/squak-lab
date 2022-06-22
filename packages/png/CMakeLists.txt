project(png)

add_library(png
	include/squak/png.h
	src/png.cpp
	src/ByteStream.h
)
target_include_directories(png PRIVATE src/ PUBLIC include/)
target_link_libraries(png compression core)