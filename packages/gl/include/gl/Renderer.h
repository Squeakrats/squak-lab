#pragma once
#include <map>
#include "SceneNode.h"
#include "glutils.h"
#include <stack>

namespace gl {

class Renderer {
private:
	GLuint program;
	std::stack<Matrix4> transforms{};
	
	// TODO - cleanup memory
	std::map<void*, GLuint> cache{};

	GLuint EnsureArrayBuffer(std::shared_ptr<Geometry::Buffer> buffer);
	GLuint EnsureElementArrayBuffer(std::shared_ptr<Geometry::BufferView> buffer);

	void RenderNode(Matrix4& camera, SceneNode& node);

public:
	void Render(Matrix4& camera, SceneNode& scene);
};

}