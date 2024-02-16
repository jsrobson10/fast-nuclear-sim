
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
};

void vertex_attrib_pointers();
glm::mat4 colour(glm::vec4 code);

};

