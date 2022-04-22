#include "gl/glutils.h"
#include "utility.h"
#include "Renderer.h"

namespace gl {

GLuint CreateShader(GLenum type, std::string source) {
	GLuint shader = glCreateShader(type);

	const char* sourceBuffer = source.data();
	GLint sourceBufferLength = static_cast<GLint>(source.length());
	glShaderSource(shader, 1, &sourceBuffer, &sourceBufferLength);
	glCompileShader(shader);

	GLint status{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetShaderInfoLog(shader, 2048, &errorBufferLength, errorBuffer);
		Panic(errorBuffer);
	}

	return shader;
}

GLuint CreateProgram(std::string vertexShaderSource, std::string fragmentShaderSource) {
	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	
	GLint status{};
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetProgramInfoLog(program, 2048, &errorBufferLength, errorBuffer);
		Panic(errorBuffer);
	}

	status = 0;
	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (!status) {
		char errorBuffer[2048] = { 0 };
		GLsizei errorBufferLength = 0;

		glGetProgramInfoLog(program, 2048, &errorBufferLength, errorBuffer);
		Panic(errorBuffer);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

std::shared_ptr<IRenderer> CreateRenderer() {
	return std::make_shared<Renderer>();
}

};