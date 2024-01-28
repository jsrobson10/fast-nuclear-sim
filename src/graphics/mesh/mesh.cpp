
#include "mesh.hpp"
#include "arrays.hpp"
#include "../shader.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

void mesh::add(const mesh& o, glm::mat4 mat)
{
	unsigned int off = vertices.size();
	glm::mat3 mat3(mat);

	vertices.reserve(vertices.size() + o.vertices.size());
	indices.reserve(indices.size() + o.indices.size());

	for(unsigned int i = 0; i < o.vertices.size(); i++)
	{
		arrays::vertex v = o.vertices[i];
		v.normal = v.normal * mat3;
		v.pos = v.pos * mat;
		vertices.push_back(v);
	}

	for(unsigned int i = 0; i < o.indices.size(); i++)
	{
		indices.push_back(o.indices[i] + off);
	}
}

void mesh::set_vertices(const arrays::vertex* data, size_t size)
{
	vertices.clear();
	vertices.reserve(size);

	for(unsigned int i = 0; i < size; i++)
	{
		vertices.push_back(data[i]);
	}
}

void mesh::set_indices(const unsigned int* data, size_t size)
{
	indices.clear();
	indices.reserve(size);

	for(unsigned int i = 0; i < size; i++)
	{
		indices.push_back(data[i]);
	}
}

