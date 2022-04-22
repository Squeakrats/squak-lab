#include "RenderingContext.h"
#include "png.h"
#include "utility.h"

namespace gl {

GLuint RenderingContext::EnsureArrayBuffer(std::shared_ptr<BufferView::Buffer> source) {
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

GLuint RenderingContext::EnsureElementArrayBuffer(std::shared_ptr<BufferView> view) {
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

GLuint RenderingContext::EnsureTexture(std::shared_ptr<Texture> texture) {
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

void RenderingContext::VertexAttribPointer(GLuint location, const BufferAccessor& accessor) {
	glBindBuffer(GL_ARRAY_BUFFER, this->EnsureArrayBuffer(accessor.view->buffer));
	glVertexAttribPointer(location, Convert(accessor.type), Convert(accessor.componentType), false, 0, (void*)(accessor.view->offset));
}

void RenderingContext::DrawElements(const BufferAccessor& accessor) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EnsureElementArrayBuffer(accessor.view));
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(accessor.view->length / sizeof(uint16_t)), Convert(accessor.componentType), nullptr);
}

};
