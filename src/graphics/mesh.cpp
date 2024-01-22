
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "arrays.hpp"
#include <iostream>

using namespace sim::graphics;

mesh::mesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	arrays::vertex_attrib_pointers();
}

mesh::mesh(mesh&& o)
{
	vbo = o.vbo;
	ebo = o.ebo;
	vao = o.vao;
	size = o.size;
	o.vbo = 0;
	o.ebo = 0;
	o.vao = 0;
}

mesh::~mesh()
{
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
	if(vao) glDeleteVertexArrays(1, &vao);
}

void mesh::set_vertices(const arrays::vertex* data, size_t size, int mode)
{
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(data[0]), data, mode);
}

void mesh::set_indices(const unsigned int* data, size_t size, int mode)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(data[0]), data, mode);
	this->size = size;
}

void mesh::bind() const
{
	glBindVertexArray(vao);
}

void mesh::render() const
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

