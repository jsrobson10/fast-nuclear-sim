
#pragma once

#include <glm/matrix.hpp>
#include <ostream>

namespace Sim::Graphics::Data::Arrays
{

struct Vertex
{
	unsigned long texid = 0;
	glm::vec2 texpos = {0, 0};
	glm::vec3 pos = {0, 0, 0};
	glm::vec3 normal = {0, 0, 0};
	glm::vec4 colour = {1, 1, 1, 1};
	glm::vec3 material = {0, 0, 0};
	float transform_id = -1;

	constexpr bool operator==(const Vertex&) const = default;

	friend std::ostream& operator<<(std::ostream& os, const Vertex& v);

} __attribute__((packed));

void vertex_attrib_pointers();

};

