#include <assert.h>
#include "gl/Renderer.h"
#include "gl/glutils.h"
#include "shaders.h"
#include "Matrix4.h"

namespace gl {

void Renderer::Render(Matrix4& camera, SceneNode& scene) {
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	this->RenderNode(camera, scene);

	GLenum error = glGetError();
	assert(error == 0);
}

void Renderer::RenderNode(Matrix4& camera, SceneNode& node) {
	if (node.geometry != nullptr) {
		if (!this->program) {
			this->program = CreateProgram(shaders::vertex, shaders::fragment);
		}

		if (this->cache.find(node.geometry.get()) == this->cache.end()) {
			Geometry& geometry = *node.geometry;

			GLuint buffer{};
			glCreateBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometry.positions.size(), geometry.positions.data(), GL_STATIC_DRAW);

			this->cache.insert(std::make_pair(node.geometry.get(), buffer));
		}

		glUseProgram(this->program);
		glEnableVertexAttribArray(0);

		GLuint buffer = this->cache.at(node.geometry.get());
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

		GLint uPerspective = glGetUniformLocation(this->program, "uPerspective");
		glUniformMatrix4fv(uPerspective, 1, false, camera.data);

		GLint uModel = glGetUniformLocation(this->program, "uModel");
		glUniformMatrix4fv(uModel, 1, false, node.transform.data);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	for (auto child : node.children) {
		this->RenderNode(camera, *child);
	}
}

};