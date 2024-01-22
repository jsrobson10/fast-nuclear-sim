
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include "shader.hpp"
#include "arrays.hpp"
#include "font.hpp"

using namespace sim::graphics;

static unsigned int vao, vbo, ebo;

static void* ptr_diff(void* a, void* b)
{
	return (void*)((size_t)a - (size_t)b);
}

void arrays::vertex_attrib_pointers()
{
	vertex v;
	
	glVertexAttribLPointer(0, 1, GL_UNSIGNED_INT64_ARB, sizeof(v), ptr_diff(&v.texid, &v));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(v), ptr_diff(&v.texpos, &v));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.pos, &v));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.normal, &v));
	glEnableVertexAttribArray(3);
}

