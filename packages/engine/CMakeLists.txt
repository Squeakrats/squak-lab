add_library(engine
	include/squak/engine/Actor.h
	include/squak/engine/Engine.h
	include/squak/engine/GLBLoader.h
	include/squak/engine/JSONLoader.h
	include/squak/engine/RuntimeTypeInfo.h
	include/squak/engine/rpc.h
	src/Actor.cpp
	src/Engine.cpp
	src/GLBLoader.cpp
	src/JSONLoader.cpp
)
target_include_directories(engine PRIVATE src/ PUBLIC include/)
target_link_libraries(engine graphics core utility json gltf window net)