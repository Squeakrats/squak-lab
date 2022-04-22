#include "Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"
#include "utility.h"
#include "png.h"
#include "BufferAccessor.h"

namespace gl {

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
		if (!this->program) {
			this->program = CreateProgram(shaders::vertex, shaders::fragment);
		}

		glUseProgram(this->program);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		Mesh& mesh = *node.mesh;
		Material& material = *mesh.material;
		Mesh::Geometry& geometry = mesh.geometry;

		this->context.VertexAttribPointer(0, *geometry.at(Mesh::AttributeType::Position));
		this->context.VertexAttribPointer(1, *geometry.at(Mesh::AttributeType::Normal));
		this->context.VertexAttribPointer(2, *geometry.at(Mesh::AttributeType::TextureCoordinate_0));

		GLint uTexture = this->context.EnsureTexture(material.baseColorTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, uTexture);

		GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
		glUniformMatrix4fv(uPerspective, 1, false, camera.perspective.data);

		GLint uView = glGetUniformLocation(this->program, "uView");
		Matrix4 view = camera.GetView();
		glUniformMatrix4fv(uView, 1, false, view.data);

		GLint uModel = glGetUniformLocation(this->program, "uModel");
		glUniformMatrix4fv(uModel, 1, false, this->transforms.top().data);

		this->context.DrawElements(*mesh.indices);
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}

	this->transforms.pop();
}

};