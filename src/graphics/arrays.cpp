
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "arrays.hpp"
#include "font.hpp"

using namespace sim::graphics;

static unsigned int vao, vbo, ebo;

static void* ptr_diff(void* a, void* b)
{
	return (void*)((size_t)a - (size_t)b);
}

unsigned int arrays::init()
{
	vertex v;
	unsigned long handle = font::chars['*'].handle;

	arrays::vertex vertices[4] = {
		{handle, {0.0f, 1.0f}, {-0.5f, -0.5f, 0.0f}},
		{handle, {0.0f, 0.0f}, {-0.5f,  0.5f, 0.0f}},
		{handle, {1.0f, 1.0f}, { 0.5f, -0.5f, 0.0f}},
		{handle, {1.0f, 0.0f}, { 0.5f,  0.5f, 0.0f}}
	};

	int indices[6] = {
		0, 1, 3, 0, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribLPointer(0, 1, GL_UNSIGNED_INT64_ARB, sizeof(v), ptr_diff(&v.texid, &v));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(v), ptr_diff(&v.texpos, &v));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.pos, &v));
	glEnableVertexAttribArray(2);

	return vao;
}

