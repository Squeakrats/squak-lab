#pragma once
#include <map>
#include "SceneNode.h"
#include "glutils.h"

namespace gl {

class Renderer {
private:
	GLuint program;
	
	// TODO - cleanup memory
	std::map<void*, GLuint> cache{};

	GLuint EnsureArrayBuffer(std::shared_ptr<Geometry::Buffer> buffer);
	GLuint EnsureElementArrayBuffer(std::shared_ptr<Geometry::BufferView> buffer);

	void RenderNode(Matrix4& camera, SceneNode& node);

public:
	void Render(Matrix4& camera, SceneNode& scene);
};

}