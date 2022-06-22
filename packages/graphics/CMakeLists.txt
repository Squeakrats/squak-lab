add_library(graphics
	include/squak/graphics/Texture.h
	include/squak/graphics/Material.h
	include/squak/graphics/BufferAccessor.h
	include/squak/graphics/CameraNode.h
	include/squak/graphics/SceneNode.h
	include/squak/graphics/Mesh.h
	include/squak/graphics/Light.h
	include/squak/graphics/Transform.h
	include/squak/graphics/IRenderer.h
	include/squak/graphics/SceneAsset.h
	src/Mesh.cpp
)
target_include_directories(graphics PRIVATE src/ PUBLIC include/)
target_link_libraries(graphics math core)