project(window)

set(SOURCES_SHARED
	src/window.h
)

if (EMSCRIPTEN)
set(SOURCES_PLATFORM
	src/window_emscripten.cpp
)
else()
set(SOURCES_PLATFORM
	src/window_win.cpp
)
endif()

add_library(window
	${SOURCES_SHARED}
	${SOURCES_PLATFORM}
)

target_include_directories(window PUBLIC src/)
target_link_libraries(window utility glr)