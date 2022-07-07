add_library(core
	include/squak/core/AssetManager.h
	include/squak/core/Buffer.h
	include/squak/core/BufferView.h
	include/squak/core/IByteStream.h
	include/squak/core/StringStream.h
	src/AssetManager.cpp
)
target_link_libraries(core utility)
target_include_directories(core PRIVATE src/ PUBLIC include/)