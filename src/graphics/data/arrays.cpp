
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <assimp/Importer.hpp>

#include "../shader.hpp"
#include "../../util/streams.hpp"
#include "arrays.hpp"
#include "font.hpp"

using namespace Sim::Graphics::Data;

static void* ptr_diff(void* a, void* b)
{
	return (void*)((size_t)a - (size_t)b);
}

void Arrays::vertex_attrib_pointers()
{
	Vertex v;

	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(v), ptr_diff(&v.texpos, &v));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.pos, &v));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(v), ptr_diff(&v.colour, &v));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, true, sizeof(v), ptr_diff(&v.tbn[0], &v));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, true, sizeof(v), ptr_diff(&v.tbn[1], &v));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, true, sizeof(v), ptr_diff(&v.tbn[2], &v));
	glEnableVertexAttribArray(5);

	glVertexAttribIPointer(6, 1, GL_INT, sizeof(v), ptr_diff(&v.transform_id, &v));
	glEnableVertexAttribArray(6);

	glVertexAttribIPointer(7, 1, GL_UNSIGNED_INT, sizeof(v), ptr_diff(&v.tex_diffuse, &v));
	glEnableVertexAttribArray(7);

	glVertexAttribIPointer(8, 1, GL_UNSIGNED_INT, sizeof(v), ptr_diff(&v.tex_normal, &v));
	glEnableVertexAttribArray(8);

	glVertexAttribPointer(9, 3, GL_FLOAT, false, sizeof(v), ptr_diff(&v.material, &v));
	glEnableVertexAttribArray(9);
}

std::ostream& Arrays::operator<<(std::ostream& os, const Vertex& v)
{
	os << "Vertex{";
	os << "texpos=" << v.texpos << ", ";
	os << "pos=" << v.pos << ", ";
	os << "colour=" << v.colour << ", ";
	os << "tbn=" << v.tbn << ", ";
	os << "transform_id=" << v.transform_id << ", ";
	os << "tex_diffuse=" << v.tex_diffuse << ", ";
	os << "tex_normal=" << v.tex_normal << ", ";
	os << "material=" << v.material;
	os << "}";
	return os;
}

