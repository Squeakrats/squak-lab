#include <assert.h>
#include "gl/Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"

namespace gl {

void Renderer::Render(Matrix4& camera, SceneNode& scene) {
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (!this->program) {
		this->program = CreateProgram(shaders::vertex, shaders::fragment);

		glGenBuffers(1, &this->buffer);
		glBindBuffer(GL_ARRAY_BUFFER, this->buffer);

		float vertices[] = {
			 100.0,  100.0,
			-100.0,  100.0,
			-100.0, -100.0,

			 100.0,  100.0,
			-100.0, -100.0,
			 100.0, -100.0,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);
	}

	glUseProgram(this->program);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

	GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
	glUniformMatrix4fv(uPerspective, 1, false, camera.data);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	GLenum error = glGetError();
	assert(error == 0);
}

};