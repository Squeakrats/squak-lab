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
			{ "aPosition", "aNormal", "aTextureCoordinate" },
			{ "uPerspective", "uView", "uModel" }
		),
		gl(gl)
	{}

	virtual void Render(Matrix4& perspective, Matrix4& view, Matrix4& model, Mesh& mesh) override {
		this->Enable();
		this->gl.VertexAttribPointer(this->attributes[0], *mesh.geometry.at(Mesh::AttributeType::Position));
		this->gl.VertexAttribPointer(this->attributes[1], *mesh.geometry.at(Mesh::AttributeType::Normal));
		this->gl.VertexAttribPointer(this->attributes[2], *mesh.geometry.at(Mesh::AttributeType::TextureCoordinate_0));
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
			{ "aPosition", "aNormal"},
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
		this->gl.VertexAttribPointer(this->attributes[1], *mesh.geometry.at(Mesh::AttributeType::Normal));
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
	this->framebuffer = gl::CreateFramebuffer(width, height, { GL_RGB, GL_RGB32F, GL_RGB32F });

	float quadBufferData[] = {
		 1.0,  1.0, -1.0,  1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0, -1.0,  1.0, -1.0,

		 1, 1, 0, 1, 0, 0,
		 1, 1, 0, 0, 1, 0
	};

	glGenBuffers(1, &this->quadBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadBufferData), quadBufferData, GL_STATIC_DRAW);
}

void Renderer::Render(CameraNode& camera, SceneNode& scene) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.framebuffer);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->RenderNode(RenderPass::Opaque, camera, scene);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	this->RenderNode(RenderPass::Light, camera, scene);

	GLenum error = glGetError();
	assert(error == 0);
}

void Renderer::RenderNode(RenderPass pass, CameraNode& camera, SceneNode& node) {
	this->transforms.push(this->transforms.top() * node.transform.ToMatrix());

	if (pass == RenderPass::Opaque && node.mesh != nullptr) {
		Matrix4 view = camera.GetView();
		if (node.mesh->material != nullptr && node.mesh->material->baseColorTexture != nullptr) {
			this->texuredRenderer->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
		else {
			this->solidRenderer->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
	}
	else if (pass == RenderPass::Light && node.light != nullptr) {
		this->RenderLight(camera, *node.light);
	}

	for (auto child : node.children) {
		this->RenderNode(pass, camera, *child);
	}

	this->transforms.pop();
}

void Renderer::RenderLight(CameraNode& camera, Light& light) {
	Vector3 lightPosition = this->transforms.top().GetPosition();

	this->lightProgram.Enable();
	glBindBuffer(GL_ARRAY_BUFFER, this->quadBuffer);
	glVertexAttribPointer(this->lightProgram.attributes[0], 2, GL_FLOAT, false, 0, nullptr);
	glVertexAttribPointer(this->lightProgram.attributes[1], 2, GL_FLOAT, false, 0, (void*)(sizeof(float) * 12));
	glUniform3f(this->lightProgram.uniforms[0], lightPosition.x, lightPosition.y, lightPosition.z);
	glUniform3f(this->lightProgram.uniforms[1], light.color.x, light.color.y, light.color.z);

	glUniform1i(this->lightProgram.uniforms[2], 0);
	glUniform1i(this->lightProgram.uniforms[3], 1);
	glUniform1i(this->lightProgram.uniforms[4], 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->framebuffer.colorAttachments[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->framebuffer.colorAttachments[1]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->framebuffer.colorAttachments[2]);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	this->lightProgram.Disable();
}

};