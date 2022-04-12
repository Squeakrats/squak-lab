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

	void RenderNode(Matrix4& camera, SceneNode& node);

public:
	void Render(Matrix4& camera, SceneNode& scene);
};

}