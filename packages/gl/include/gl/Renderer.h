#pragma once
#include "SceneNode.h"
#include "glutils.h"

namespace gl {

class Renderer {
private:
	GLuint program;
	GLuint buffer;

public:
	void Render(SceneNode& scene);
};

}