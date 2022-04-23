#pragma once
#include <memory>
#include <map>
#include "gl/glutils.h"
#include "BufferAccessor.h"
#include "Texture.h"

namespace gl {

class RenderingContext {
	// TODO - cleanup memory
	std::map<void*, GLuint> cache{};

public:
	GLuint EnsureArrayBuffer(std::shared_ptr<BufferView::Buffer> buffer);
	GLuint EnsureElementArrayBuffer(std::shared_ptr<BufferView> buffer);
	GLuint EnsureTexture(std::shared_ptr<Texture> texture);

	void VertexAttribPointer(GLuint location, const BufferAccessor& accessor);
	void SetActiveTexture(GLenum location, std::shared_ptr<Texture> texture);
	void DrawElements(const BufferAccessor& accessor);
};

};
