set(SOURCES
	include/squak/compression/inflate.h
	include/squak/compression/base64.h
	include/squak/compression/sha1.h
	src/BitStream.h
	src/inflate.cpp
	src/base64.cpp)

if (NOT EMSCRIPTEN)
list(APPEND SOURCES src/sha1.cpp)
endif()

add_library(compression ${SOURCES})
target_include_directories(compression PRIVATE src/ PUBLIC include/)
target_link_libraries(compression utility core)

add_executable(CompressionTests src/tests/CompressionTests.cpp)
target_link_libraries(CompressionTests gtest_main compression)
set_target_properties(CompressionTests PROPERTIES FOLDER test)