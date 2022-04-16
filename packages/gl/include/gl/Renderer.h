#pragma once
#include <map>
#include "SceneNode.h"
#include "glutils.h"
#include <stack>
#include "IRenderer.h"

namespace gl {

class Renderer : public IRenderer {
private:
	GLuint program{};
	std::stack<Matrix4> transforms{};
	
	// TODO - cleanup memory
	std::map<void*, GLuint> cache{};

	GLuint EnsureArrayBuffer(std::shared_ptr<Geometry::Buffer> buffer);
	GLuint EnsureElementArrayBuffer(std::shared_ptr<Geometry::BufferView> buffer);
	void MapAttribute(GLuint location, const Geometry::Accessor& accessor);

	void RenderNode(CameraNode& camera, SceneNode& node);

public:
	Renderer() {
		this->transforms.push(Matrix4::Identity());
	}

	virtual void Render(CameraNode& camera, SceneNode& scene) override;
};

}