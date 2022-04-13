#include <assert.h>
#include "gl/Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"

namespace gl {

GLenum Convert(Geometry::ComponentType type) {
	switch (type) {
		case Geometry::ComponentType::Float:
			return GL_FLOAT;
		case Geometry::ComponentType::UnsignedInt:
			return GL_UNSIGNED_INT;
		case Geometry::ComponentType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		default:
			assert(false);
			throw std::exception();
	}
}

GLenum Convert(Geometry::AccessorType type) {
	switch (type) {
		case Geometry::AccessorType::Scalar:
			return 1;
		case Geometry::AccessorType::Vector2:
			return 2;
		case Geometry::AccessorType::Vector3:
			return 3;
		default:
			assert(false);
			throw std::exception();
	}
}


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


GLuint Renderer::EnsureElementArrayBuffer(std::shared_ptr<Geometry::BufferView> view) {
	auto find = this->cache.find(view.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint buffer{};
	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, view->length, view->buffer->data() + view->offset, GL_STATIC_DRAW);

	this->cache.insert(std::make_pair(view.get(), buffer));

	return buffer;
}

void Renderer::Render(Matrix4& camera, SceneNode& scene) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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
		glEnableVertexAttribArray(1);

		auto& position = geometry.attributes.at(Geometry::AttributeType::Position);
		glBindBuffer(GL_ARRAY_BUFFER, this->EnsureArrayBuffer(position->view->buffer));
		glVertexAttribPointer(0, Convert(position->type), Convert(position->componentType), false, 0, nullptr);

		auto& normal = geometry.attributes.at(Geometry::AttributeType::Normal);
		glBindBuffer(GL_ARRAY_BUFFER, this->EnsureArrayBuffer(normal->view->buffer));
		glVertexAttribPointer(1, Convert(normal->type), Convert(normal->componentType), false, 0, (void*)(normal->view->offset));

		GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
		glUniformMatrix4fv(uPerspective, 1, false, camera.data);

		GLint uModel = glGetUniformLocation(this->program, "uModel");
		glUniformMatrix4fv(uModel, 1, false, node.transform.ToMatrix().data);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EnsureElementArrayBuffer(geometry.indices->view));
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(geometry.indices->view->length / sizeof(uint16_t)), Convert(geometry.indices->componentType), nullptr);
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}
}

};