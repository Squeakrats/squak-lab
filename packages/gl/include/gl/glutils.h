#pragma once
#include <string>
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <gl/glew.h>
#endif
#include "IRenderer.h"

namespace gl {

GLuint CreateShader(GLenum type, std::string source);
GLuint CreateProgram(std::string vertexShaderSource, std::string fragmentShaderSource);
std::shared_ptr<IRenderer> CreateRenderer(uint32_t width, uint32_t height);

};