
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glmesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

constexpr static void init(glmesh* m)
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

glmesh::glmesh(glmesh&& o)
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

glmesh::~glmesh()
{
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
	if(vao) glDeleteVertexArrays(1, &vao);
}

void glmesh::bind()
{
	init(this);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void glmesh::uniform()
{
	glm::mat4 m = camera::get_matrix() * model_matrix;
	glUniformMatrix4fv(shader::gl_model, 1, false, &m[0][0]);
	glUniformMatrix4fv(shader::gl_tex_mat, 1, false, &colour_matrix[0][0]);
}

void glmesh::set(const mesh& m, int mode)
{
	glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(m.vertices[0]), &m.vertices[0], mode);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(m.indices[0]), &m.indices[0], mode);
	this->size = m.indices.size();
}

void glmesh::render()
{
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

