
#pragma once

#include <glm/matrix.hpp>

namespace Sim::Graphics::Arrays
{

struct Vertex
{
	unsigned long texid = 0;
	glm::vec2 texpos = {0, 0};
	glm::vec4 pos = {0, 0, 0, 1};
	glm::vec3 normal = {0, 0, 0};
	glm::vec4 colour = {1, 1, 1, 1};
	glm::vec3 material = {0, 0, 0};
	float transform_id = -1;

	constexpr bool operator==(const Vertex&) const = default;

} __attribute__((packed));

void vertex_attrib_pointers();

};

