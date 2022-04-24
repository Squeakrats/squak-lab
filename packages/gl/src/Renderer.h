#pragma once
#include <map>
#include "SceneNode.h"
#include "gl/glutils.h"
#include <stack>
#include "IRenderer.h"
#include "RenderingContext.h"
#include "Program.h"

namespace gl {

class Renderer : public IRenderer {
private:
	RenderingContext context{};
	std::shared_ptr<Program> texturedProgram{};
	std::shared_ptr<Program> solidProgram{};

	GLuint framebuffer{};
	GLuint colorTexture{};
	GLuint quadProgram{};
	GLuint quadPositions{};
	GLuint quadTextureCoordinates{};

	std::stack<Matrix4> transforms{{ Matrix4::Identity() }};

	void RenderNode(CameraNode& camera, SceneNode& node);

public:
	Renderer(uint32_t width, uint32_t height);

	virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}