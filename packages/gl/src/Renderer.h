#pragma once
#include <map>
#include "SceneNode.h"
#include "gl/glutils.h"
#include <stack>
#include "IRenderer.h"
#include "RenderingContext.h"

namespace gl {

class Renderer : public IRenderer {
private:
	RenderingContext context{};
	GLuint program{};
	std::stack<Matrix4> transforms{{ Matrix4::Identity() }};

	void RenderNode(CameraNode& camera, SceneNode& node);

public:
	virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}