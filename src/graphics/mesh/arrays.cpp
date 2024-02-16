
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include "../shader.hpp"
#include "arrays.hpp"
#include "font.hpp"

using namespace Sim::Graphics;

static unsigned int vao, vbo, ebo;

static void* ptr_diff(void* a, void* b)
{
	return (void*)((size_t)a - (size_t)b);
}

void Arrays::vertex_attrib_pointers()
{
	Vertex v;
	
	glVertexAttribLPointer(0, 1, GL_UNSIGNED_INT64_ARB, sizeof(v), ptr_diff(&v.texid, &v));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(v), ptr_diff(&v.texpos, &v));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(v), ptr_diff(&v.pos, &v));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.normal, &v));
	glEnableVertexAttribArray(3);
}

glm::mat4 Arrays::colour(glm::vec4 c)
{
	return glm::mat4({
		c.r, c.g, c.b, c.a,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	});
}

