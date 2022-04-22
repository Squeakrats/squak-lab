#include "gl/Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"
#include "utility.h"
#include "png.h"
#include "BufferAccessor.h"

namespace gl {

GLenum Convert(BufferAccessor::ComponentType type) {
	switch (type) {
		case BufferAccessor::ComponentType::Float:
			return GL_FLOAT;
		case BufferAccessor::ComponentType::UnsignedInt:
			return GL_UNSIGNED_INT;
		case BufferAccessor::ComponentType::UnsignedShort:
			return GL_UNSIGNED_SHORT;
		default:
			Panic(std::format("unsupported type {}!", static_cast<int>(type)));
	}
}

GLenum Convert(BufferAccessor::Type type) {
	switch (type) {
		case BufferAccessor::Type::Scalar:
			return 1;
		case BufferAccessor::Type::Vector2:
			return 2;
		case BufferAccessor::Type::Vector3:
			return 3;
		default:
			Panic(std::format("unsupported type {}!", static_cast<int>(type)));
	}
}


GLuint Renderer::EnsureArrayBuffer(std::shared_ptr<BufferView::Buffer> source) {
	auto find = this->cache.find(source.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint buffer{};
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, source->size(), source->data(), GL_STATIC_DRAW);

	this->cache.insert(std::make_pair(source.get(), buffer));

	return buffer;
}

GLuint Renderer::EnsureElementArrayBuffer(std::shared_ptr<BufferView> view) {
	auto find = this->cache.find(view.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint buffer{};
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, view->length, view->buffer->data() + view->offset, GL_STATIC_DRAW);

	this->cache.insert(std::make_pair(view.get(), buffer));

	return buffer;
}

GLuint Renderer::EnsureTexture(std::shared_ptr<Texture> texture) {
	auto find = this->cache.find(texture.get());
	if (find != this->cache.end()) {
		return find->second;
	}

	GLuint textureId{};
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// TODO - avoid this copy
	std::vector<uint8_t> buffer{};
	for (size_t i = 0; i < texture->image->length; i++) {
		buffer.push_back((*texture->image->buffer)[texture->image->offset + i]);
	}

	png::PNG png = png::parse(buffer);
	GLint format = (png.header.colorType == png::COLOR_TYPE_RGB) ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, png.header.width, png.header.height, 0, format, GL_UNSIGNED_BYTE, png.data.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	this->cache.insert(std::make_pair(texture.get(), textureId));

	return textureId;
}

void Renderer::MapAttribute(GLuint location, const BufferAccessor& accessor) {
	glBindBuffer(GL_ARRAY_BUFFER, this->EnsureArrayBuffer(accessor.view->buffer));
	glVertexAttribPointer(location, Convert(accessor.type), Convert(accessor.componentType), false, 0, (void*)(accessor.view->offset));
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

		this->MapAttribute(0, *geometry.at(Mesh::AttributeType::Position));
		this->MapAttribute(1, *geometry.at(Mesh::AttributeType::Normal));
		this->MapAttribute(2, *geometry.at(Mesh::AttributeType::TextureCoordinate_0));

		GLint uTexture = this->EnsureTexture(material.baseColorTexture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, uTexture);

		GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
		glUniformMatrix4fv(uPerspective, 1, false, camera.perspective.data);

		GLint uView = glGetUniformLocation(this->program, "uView");
		Matrix4 view = camera.GetView();
		glUniformMatrix4fv(uView, 1, false, view.data);

		GLint uModel = glGetUniformLocation(this->program, "uModel");
		glUniformMatrix4fv(uModel, 1, false, this->transforms.top().data);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EnsureElementArrayBuffer(mesh.indices->view));
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices->view->length / sizeof(uint16_t)), Convert(mesh.indices->componentType), nullptr);
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}

	this->transforms.pop();
}

};