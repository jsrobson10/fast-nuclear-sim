
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sim::graphics::arrays
{

struct vertex
{
	unsigned long texid;
	glm::vec2 texpos;
	glm::vec3 pos;
};

unsigned int init();

};

