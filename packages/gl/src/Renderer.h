#pragma once
#include <map>
#include "SceneNode.h"
#include "gl/glutils.h"
#include <stack>
#include "IRenderer.h"
#include "RenderingContext.h"
#include "Program.h"
#include "shaders.h"

namespace gl {

class AMeshRenderer : public Program {
public:
	AMeshRenderer(std::string vertexSource, std::string fragmentSource, std::vector<std::string> attributes, std::vector<std::string> uniforms)
		: Program(vertexSource, fragmentSource, attributes, uniforms) {}

	virtual void Render(Matrix4& perspective, Matrix4& view, Matrix4& model, Mesh& mesh) = 0;
};

enum class RenderPass {
	Opaque,
	Light
};

class Renderer : public IRenderer {
private:
	RenderingContext context{};
	std::shared_ptr<AMeshRenderer> texuredRenderer{};
	std::shared_ptr<AMeshRenderer> solidRenderer{};

	gl::Framebuffer framebuffer{};
	Program lightProgram = Program(
		shaders::fullscreen::vertex,
		shaders::fullscreen::fragment,
		{ "aPosition", "aTextureCoordinate" },
		{ "uLightPosition", "uLightColor", "uDiffuseTexture", "uPositionTexture", "uNormalTexture" }
	);
	GLuint quadBuffer{};

	std::stack<Matrix4> transforms{{ Matrix4::Identity() }};

	void RenderNode(RenderPass pass, CameraNode& camera, SceneNode& node);
	void RenderLight(CameraNode& camera, Light& light);

public:
	Renderer(uint32_t width, uint32_t height);

	virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}