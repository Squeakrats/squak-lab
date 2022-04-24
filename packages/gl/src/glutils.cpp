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

Framebuffer CreateFramebuffer(uint32_t width, uint32_t height, std::vector<GLenum> colorTypes) {
	GLsizei nColorAttachments = (GLsizei)colorTypes.size();

	Framebuffer framebuffer{};

	glGenFramebuffers(1, &framebuffer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.framebuffer);

	glGenRenderbuffers(1, &framebuffer.depthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, framebuffer.depthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebuffer.depthAttachment);

	framebuffer.colorAttachments.resize(nColorAttachments);
	glGenTextures(nColorAttachments, framebuffer.colorAttachments.data());
	for (GLsizei i = 0; i < nColorAttachments; i++) {
		glBindTexture(GL_TEXTURE_2D, framebuffer.colorAttachments[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, colorTypes[i], nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, framebuffer.colorAttachments[i], 0);
	}

	std::vector<GLenum> drawBuffers(nColorAttachments);
	for (GLsizei i = 0; i < nColorAttachments; i++) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	glDrawBuffers(nColorAttachments, drawBuffers.data());

	Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Invalid framebuffer");

	return framebuffer;
}

std::shared_ptr<IRenderer> CreateRenderer(uint32_t width, uint32_t height) {
	return std::make_shared<Renderer>(width, height);
}

};