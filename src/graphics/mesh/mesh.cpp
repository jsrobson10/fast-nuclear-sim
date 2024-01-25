
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

constexpr static void init(mesh* m)
{
	if(m->vao != 0)
	{
		return;
	}

	glGenVertexArrays(1, &m->vao);
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ebo);

	glBindVertexArray(m->vao);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	arrays::vertex_attrib_pointers();
}

mesh::mesh(mesh&& o)
{
	vbo = o.vbo;
	ebo = o.ebo;
	vao = o.vao;
	size = o.size;
	colour_matrix = o.colour_matrix;
	model_matrix = o.model_matrix;

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

void mesh::bind()
{
	init(this);

	glm::mat4 m = camera::get_matrix() * model_matrix;
	glUniformMatrix4fv(shader::gl_model, 1, false, &m[0][0]);
	glUniformMatrix4fv(shader::gl_tex_mat, 1, false, &colour_matrix[0][0]);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void mesh::render()
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

