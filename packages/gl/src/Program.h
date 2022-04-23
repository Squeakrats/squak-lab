#pragma once
#include "gl/glutils.h"
#include <vector>

namespace gl {

class Program {
public:
	struct Context {
		Matrix4& perspective;
		Matrix4& view;
		Matrix4& model;
		Mesh& mesh;
	};

	GLuint program{};
	std::vector<GLuint> attributes{};
	std::vector<GLuint> uniforms{};

public:
	Program(std::string vertexSource, std::string fragmentSource, std::vector<std::string> attributes, std::vector<std::string> uniforms) {
		this->program = gl::CreateProgram(vertexSource, fragmentSource);

		for (std::string attribute : attributes) {
			this->attributes.push_back(glGetAttribLocation(program, attribute.c_str()));
		}

		for (std::string uniform : uniforms) {
			this->uniforms.push_back(glGetUniformLocation(program, uniform.c_str()));
		}
	}

	virtual void Render(Matrix4& perspective, Matrix4& view, Matrix4& model, Mesh& mesh) = 0;

	void Enable() {
		glUseProgram(this->program);
		for (GLuint i = 0; i < this->attributes.size(); i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void Disable() {
		for (GLuint i = 0; i < this->attributes.size(); i++) {
			glDisableVertexAttribArray(i);
		}
	}
};

};
