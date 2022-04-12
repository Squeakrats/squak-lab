#include <assert.h>
#include "gl/Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"

namespace gl {

GLuint Renderer::EnsureArrayBuffer(std::shared_ptr<Geometry::Buffer> source) {
	auto find = this->cache.find(source.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint buffer{};
	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, source->size(), source->data(), GL_STATIC_DRAW);

	this->cache.insert(std::make_pair(source.get(), buffer));

	return buffer;
}


GLuint Renderer::EnsureElementArrayBuffer(std::shared_ptr<Geometry::BufferView> source) {
	auto find = this->cache.find(source.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint buffer{};
	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, source->length, source->buffer->data() + source->offset, GL_STATIC_DRAW);

	this->cache.insert(std::make_pair(source.get(), buffer));

	return buffer;
}

void Renderer::Render(Matrix4& camera, SceneNode& scene) {
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	this->RenderNode(camera, scene);

	GLenum error = glGetError();
	assert(error == 0);
}

void Renderer::RenderNode(Matrix4& camera, SceneNode& node) {
	if (node.geometry != nullptr) {
		Geometry& geometry = *node.geometry;

		if (!this->program) {
			this->program = CreateProgram(shaders::vertex, shaders::fragment);
		}

		glUseProgram(this->program);
		glEnableVertexAttribArray(0);

		auto& position = geometry.attributes.at(Geometry::AttributeType::Position);
		GLuint buffer = this->EnsureArrayBuffer(position->view->buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

		GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
		glUniformMatrix4fv(uPerspective, 1, false, camera.data);

		GLint uModel = glGetUniformLocation(this->program, "uModel");
		glUniformMatrix4fv(uModel, 1, false, node.transform.data);

		GLuint indexBuffer = this->EnsureElementArrayBuffer(geometry.indices->view);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(geometry.indices->count), GL_UNSIGNED_INT, nullptr);
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}
}

};