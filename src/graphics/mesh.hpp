
#pragma once

#include "arrays.hpp"

namespace sim::graphics
{

struct mesh
{
	unsigned int vao = 0, vbo = 0, ebo = 0, size = 0;

	mesh();
	mesh(mesh&& o);
	mesh(const mesh& o) = delete;
	~mesh();

	void bind() const;
	void set_vertices(const arrays::vertex* data, size_t size, int mode);
	void set_indices(const unsigned int* data, size_t size, int mode);
	void render() const;
};

};

