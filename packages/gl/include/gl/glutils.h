#pragma once
#include <string>
#include <glr.h>
#include "IRenderer.h"

namespace gl {

struct Framebuffer {
  GLuint framebuffer{};
  std::vector<GLuint> colorAttachments{};
  GLuint depthAttachment{};
};

GLuint CreateShader(GLenum type, std::string source);
GLuint CreateProgram(std::string vertexShaderSource,
                     std::string fragmentShaderSource);
Framebuffer CreateFramebuffer(uint32_t width,
                              uint32_t height,
                              std::vector<GLenum> attachments);

std::shared_ptr<IRenderer> CreateRenderer(uint32_t width, uint32_t height);

};