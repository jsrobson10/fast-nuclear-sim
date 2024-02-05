
#pragma once

#include <string>

#include "arrays.hpp"
#include "mesh.hpp"

#include <glm/matrix.hpp>

namespace sim::graphics
{

struct glmesh
{
	unsigned int vao = 0, vbo = 0, ebo = 0, size = 0;

	glm::mat4 model_matrix {1.0f};
	glm::mat4 colour_matrix {1.0f};

	constexpr glmesh() { }

	glmesh(glmesh&& o);
	glmesh(const glmesh& o) = delete;
	~glmesh();

	void bind();
	void uniform();
	void set(const mesh& m, int mode);
	void render(int type);
	void render();
};

};

