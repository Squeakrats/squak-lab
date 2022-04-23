#include "Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"
#include "utility.h"
#include "png.h"
#include "BufferAccessor.h"
#include "shaders.h"

namespace gl {

class TexturedMaterialProgram : public Program {
private:
	RenderingContext& gl;

public:
	TexturedMaterialProgram(RenderingContext& gl) : 
		Program(
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


class SolidMaterialProgram : public Program {
private:
	RenderingContext& gl;

public:
	SolidMaterialProgram(RenderingContext& gl) :
		Program(
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

Renderer::Renderer() {
	this->texturedProgram = std::make_shared<TexturedMaterialProgram>(this->context);
	this->solidProgram = std::make_shared<SolidMaterialProgram>(this->context);
}

void Renderer::Render(CameraNode& camera, SceneNode& scene) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	this->RenderNode(camera, scene);

	GLenum error = glGetError();
	assert(error == 0);
}

void Renderer::RenderNode(CameraNode& camera, SceneNode& node) {
	this->transforms.push(this->transforms.top() * node.transform.ToMatrix());

	if (node.mesh != nullptr) {
		Matrix4 view = camera.GetView();

		if (node.mesh->material != nullptr && node.mesh->material->baseColorTexture != nullptr) {
			this->texturedProgram->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
		else {
			this->solidProgram->Render(camera.perspective, view, this->transforms.top(), *node.mesh);
		}
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}

	this->transforms.pop();
}

};