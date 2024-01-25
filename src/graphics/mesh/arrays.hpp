
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sim::graphics::arrays
{

struct vertex
{
	unsigned long texid = 0;
	glm::vec2 texpos = {0, 0};
	glm::vec4 pos = {0, 0, 0, 1};
	glm::vec3 normal = {0, 0, 0};
};

void vertex_attrib_pointers();

};

