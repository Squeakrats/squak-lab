#include <assert.h>
#include "gl/glutils.h"

namespace gl {

GLuint CreateShader(GLenum type, std::string source) {
	GLuint shader = glCreateShader(type);

	const char* sourceBuffer = source.data();
	GLint sourceBufferLength = source.length();
	glShaderSource(shader, 1, &sourceBuffer, &sourceBufferLength);
	glCompileShader(shader);

	GLint status{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetShaderInfoLog(shader, 2048, &errorBufferLength, errorBuffer);
		assert(false);
	}

	return shader;
}

GLuint CreateProgram(std::string vertexShaderSource, std::string fragmentShaderSource) {
	GLuint program = glCreateProgram();
	glAttachShader(program, CreateShader(GL_VERTEX_SHADER, vertexShaderSource));
	glAttachShader(program, CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource));
	
	GLint status{};
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetProgramInfoLog(program, 2048, &errorBufferLength, errorBuffer);
		assert(false);
	}

	status = 0;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetProgramInfoLog(program, 2048, &errorBufferLength, errorBuffer);
		assert(false);
	}

	return program;
}

};