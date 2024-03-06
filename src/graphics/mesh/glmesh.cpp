
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glmesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"
#include "../../util/streams.hpp"

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
	ssbo = o.ssbo;

	o.vbo = 0;
	o.ebo = 0;
	o.vao = 0;
	o.ssbo = 0;
	
	size = o.size;
	ssbo_size = o.ssbo_size;
}

GLMesh::~GLMesh()
{
	if(vbo) glDeleteBuffers(1, &vbo);
	if(ebo) glDeleteBuffers(1, &ebo);
	if(ssbo) glDeleteBuffers(1, &ssbo);
	if(vao) glDeleteVertexArrays(1, &vao);
}

void GLMesh::bind(bool bind_ssbo)
{
	init(this);

	glBindVertexArray(vao);

	if(ssbo_size > 0 && bind_ssbo)
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	}
}

void GLMesh::set(const Mesh& m, int mode, bool send_ssbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ARRAY_BUFFER, m.vertices.size() * sizeof(m.vertices[0]), &m.vertices[0], mode);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m.indices.size() * sizeof(m.indices[0]), &m.indices[0], mode);

	this->size = m.indices.size();
	this->ssbo_size = m.transforms.size();

	if(ssbo_size > 0 && send_ssbo)
	{
		if(ssbo == 0)
		{
			glGenBuffers(1, &ssbo);
		}

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m.transforms.size() * sizeof(m.transforms[0]), &m.transforms[0], mode);
	}
}

void GLMesh::render()
{
	render(GL_TRIANGLES);
}

void GLMesh::render(int type)
{
	glDrawElements(type, size, GL_UNSIGNED_INT, 0);
}

