
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include "../shader.hpp"
#include "../../util/streams.hpp"
#include "arrays.hpp"
#include "font.hpp"

using namespace Sim::Graphics;

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

	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.pos, &v));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.normal, &v));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(v), ptr_diff(&v.colour, &v));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.material, &v));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 1, GL_FLOAT, false, sizeof(v), ptr_diff(&v.transform_id, &v));
	glEnableVertexAttribArray(6);
}

std::ostream& Arrays::operator<<(std::ostream& os, const Vertex& v)
{
	os << "Vertex{";
	os << "texid=" << v.texid << ", ";
	os << "texpos=" << v.texpos << ", ";
	os << "pos=" << v.pos << ", ";
	os << "normal=" << v.normal << ", ";
	os << "colour=" << v.colour << ", ";
	os << "material=" << v.material << ", ";
	os << "transform_id=" << v.transform_id;
	os << "}";
	return os;
}

