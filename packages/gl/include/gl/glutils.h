#pragma once
#include <string>
#ifdef EMSCRIPTEN
#include <GLES3/gl3.h>
#else
#include <gl/glew.h>
#endif

namespace gl {

GLuint CreateShader(GLenum type, std::string source);
GLuint CreateProgram(std::string vertexShaderSource, std::string fragmentShaderSource);

};