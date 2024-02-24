
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glmesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"

using namespace Sim::Graphics;

constexpr static void init(GLMesh* m)
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

	Arrays::vertex_attrib_pointers();
}

GLMesh::GLMesh(GLMesh&& o)
{
	vbo = o.vbo;
	ebo = o.ebo;
	vao = o.vao;
	size = o.size;
	model_matrix = o.model_matrix;

	o.vbo = 0;
	o.ebo = 0;
	o.vao = 0;
}

GLMesh::~GLMesh()
{
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
	if(vao) glDeleteVertexArrays(1, &vao);
}

void GLMesh::bind()
{
	init(this);

	glBindVertexArray(vao);
}

void GLMesh::uniform()
{
	glUniformMatrix4fv(Shader::MAIN["model"], 1, false, &model_matrix[0][0]);
}

void GLMesh::set(const Mesh& m, int mode)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(m.vertices[0]), &m.vertices[0], mode);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(m.indices[0]), &m.indices[0], mode);
	this->size = m.indices.size();
}

void GLMesh::render()
{
	render(GL_TRIANGLES);
}

void GLMesh::render(int type)
{
	glDrawElements(type, size, GL_UNSIGNED_INT, 0);
}

