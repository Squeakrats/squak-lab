#include "Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"
#include "utility.h"
#include "png.h"
#include "BufferAccessor.h"
#include "shaders.h"

namespace gl {

class TexturedRenderer : public AMeshRenderer {
private:
	RenderingContext& gl;

public:
	TexturedRenderer(RenderingContext& gl) :
		AMeshRenderer(
			shaders::textured::vertex, shaders::textured::fragment,
			{ "aPosition", "aTextureCoordinate" },
			{ "uPerspective", "uView", "uModel" }
		),
		gl(gl)
	{}

	virtual void Render(Matrix4& perspective, Matrix4& view, Matrix4& model, Mesh& mesh) override {
		this->Enable();
		this->gl.VertexAttribPointer(this->attributes[0], *mesh.geometry.at(Mesh::AttributeType::Position));
		this->gl.VertexAttribPointer(this->attributes[1], *mesh.geometry.at(Mesh::AttributeType::TextureCoordinate_0));
		this->gl.SetActiveTexture(0, mesh.material->baseColorTexture);
		glUniformMatrix4fv(this->uniforms[0], 1, false, perspective.data);
		glUniformMatrix4fv(this->uniforms[1], 1, false, view.data);
		glUniformMatrix4fv(this->uniforms[2], 1, false, model.data);
		this->gl.DrawElements(*mesh.indices);
		this->Disable();
	}
};


class SolidRenderer : public AMeshRenderer {
private:
	RenderingContext& gl;

public:
	SolidRenderer(RenderingContext& gl) :
		AMeshRenderer(
			shaders::solid::vertex, shaders::solid::fragment,
			{ "aPosition" },
			{ "uPerspective", "uView", "uModel", "uColor"}
		),
		gl(gl)
	{}

	virtual void Render(Matrix4& perspective, Matrix4& view, Matrix4& model, Mesh& mesh) override {
		Vector3 baseColor(1, 0, 0);
		if (mesh.material != nullptr) {
			baseColor = mesh.material->baseColor;
		}

		this->Enable();
		this->gl.VertexAttribPointer(this->attributes[0], *mesh.geometry.at(Mesh::AttributeType::Position));
		glUniformMatrix4fv(this->uniforms[0], 1, false, perspective.data);
		glUniformMatrix4fv(this->uniforms[1], 1, false, view.data);
		glUniformMatrix4fv(this->uniforms[2], 1, false, model.data);
		glUniform3f(this->uniforms[3], baseColor.x, baseColor.y, baseColor.z);
		this->gl.DrawElements(*mesh.indices);
		this->Disable();
	}
};

Renderer::Renderer(uint32_t width, uint32_t height) {
	this->texuredRenderer = std::make_shared<TexturedRenderer>(this->context);
	this->solidRenderer = std::make_shared<SolidRenderer>(this->context);

	glGenTextures(1, &this->colorTexture);
	glBindTexture(GL_TEXTURE_2D, this->colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLuint depthRenderbuffer;
	glGenRenderbuffers(1, &depthRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->colorTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawbuffers);

	Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Invalid framebuffer");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float positions[] = {
		 1.0,  1.0, -1.0,  1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0, -1.0,  1.0, -1.0
	};

	float textureCoordinates[] = {
		1, 1, 0, 1, 0, 0,
		1, 1, 0, 0, 1, 0
	};

	glGenBuffers(1, &this->quadPositions);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadPositions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &this->quadTextureCoordinates);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadTextureCoordinates);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);
}

void Renderer::Render(CameraNode& camera, SceneNode& scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->RenderNode(camera, scene);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	this->quadProgram.Enable();

	glBindBuffer(GL_ARRAY_BUFFER, this->quadPositions);
	glVertexAttribPointer(this->quadProgram.attributes[0], 2, GL_FLOAT, false, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, this->quadTextureCoordinates);
	glVertexAttribPointer(this->quadProgram.attributes[1], 2, GL_FLOAT, false, 0, nullptr);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->colorTexture);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->quadProgram.Disable();

	GLenum error = glGetError();
	assert(error == 0);
}

void Renderer::RenderNode(CameraNode& camera, SceneNode& node) {
	this->transforms.push(this->transforms.top() * node.transform.ToMatrix());

	if (node.mesh != nullptr) {
		Matrix4 view = camera.GetView();

		if (node.mesh->material != nullptr && node.mesh->material->baseColorTexture != nullptr) {
			this->texuredRenderer->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
		else {
			this->solidRenderer->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}

	this->transforms.pop();
}

};