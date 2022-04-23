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

	std::stack<Matrix4> transforms{{ Matrix4::Identity() }};

	void RenderNode(CameraNode& camera, SceneNode& node);

public:
	Renderer();

	virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}