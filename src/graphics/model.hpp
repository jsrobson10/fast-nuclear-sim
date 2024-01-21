
#pragma once

#include "arrays.hpp"

namespace sim::graphics
{

struct model
{
	unsigned int vao = 0, vbo = 0, ebo = 0, size = 0;

	model();
	~model();

	void bind();
	void alloc();
	void load(const char* path);
	void load_vbo(const arrays::vertex* data, size_t size, int mode);
	void load_ebo(const unsigned int* data, size_t size, int mode);
	void render();
};

};

