cmake_minimum_required(VERSION 3.8)
project(graphics)

add_library(graphics
	src/Texture.h
	src/Material.h
	src/BufferAccessor.h
	src/BufferView.h
	src/CameraNode.h
	src/SceneNode.h
	src/Geometry.h
	src/Geometry.cpp
	src/Transform.h
	src/IRenderer.h
	src/SceneAsset.h
)
target_include_directories(graphics PUBLIC src/)
target_link_libraries(graphics math core)