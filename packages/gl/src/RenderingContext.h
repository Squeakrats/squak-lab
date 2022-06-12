#pragma once
#include <squak/graphics/BufferAccessor.h>
#include <squak/graphics/Texture.h>
#include <squak/gl/glutils.h>
#include <map>
#include <memory>

namespace gl {

class RenderingContext {
  // TODO - cleanup memory
  std::map<void*, GLuint> cache{};

public:
  GLuint EnsureArrayBuffer(std::shared_ptr<Buffer> buffer);
  GLuint EnsureElementArrayBuffer(std::shared_ptr<BufferView> buffer);
  GLuint EnsureTexture(std::shared_ptr<Texture> texture);

  void VertexAttribPointer(GLuint location, const BufferAccessor& accessor);
  void SetActiveTexture(GLenum location, std::shared_ptr<Texture> texture);
  void DrawElements(const BufferAccessor& accessor);
};

};
