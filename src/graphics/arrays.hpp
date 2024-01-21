
#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sim::graphics::arrays
{

struct vertex
{
	unsigned long texid = 0;
	glm::vec2 texpos = {0, 0};
	glm::vec3 pos = {0, 0, 0};

	vertex() { }
	vertex(unsigned long texid, glm::vec2 texpos, glm::vec3 pos) : texid(texid), texpos(texpos), pos(pos) { }
};

void vertex_attrib_pointers();

};

