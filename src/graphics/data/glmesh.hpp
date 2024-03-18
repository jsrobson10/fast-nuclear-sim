
#pragma once

#include <string>

#include "arrays.hpp"
#include "mesh.hpp"

#include <glm/matrix.hpp>

namespace Sim::Graphics::Data
{

struct GLMesh
{
	unsigned int vao = 0, vbo = 0, ebo = 0, ssbo = 0;
	int ssbo_size = 0;
	int size = 0;

	constexpr GLMesh() { }

	GLMesh(GLMesh&& o);
	GLMesh(const GLMesh& o) = delete;
	~GLMesh();

	void bind();
	void bind_ssbo();
	void set(const Mesh& m, int mode);
	void render(int type);
	void render();
};

};

