
#pragma once

#include <string>

#include "arrays.hpp"
#include "mesh.hpp"

#include <glm/matrix.hpp>

namespace sim::graphics
{

struct GLMesh
{
	unsigned int vao = 0, vbo = 0, ebo = 0, size = 0;

	glm::mat4 model_matrix {1.0f};
	glm::mat4 colour_matrix {1.0f};

	constexpr GLMesh() { }

	GLMesh(GLMesh&& o);
	GLMesh(const GLMesh& o) = delete;
	~GLMesh();

	void bind();
	void uniform();
	void set(const Mesh& m, int mode);
	void render(int type);
	void render();
};

};

