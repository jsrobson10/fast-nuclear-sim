
#pragma once

#include <string>

#include "arrays.hpp"
#include <glm/matrix.hpp>

namespace sim::graphics
{

struct mesh
{
	unsigned int vao = 0, vbo = 0, ebo = 0, size = 0;

	glm::mat4 model_matrix {1.0f};
	glm::mat4 colour_matrix {1.0f};

	constexpr mesh() { }

	mesh(mesh&& o);
	mesh(const mesh& o) = delete;
	~mesh();

	void bind();
	void set_vertices(const arrays::vertex* data, size_t size, int mode);
	void set_indices(const unsigned int* data, size_t size, int mode);
	void load_model(std::string base, std::string path);
	void render();
};

};

