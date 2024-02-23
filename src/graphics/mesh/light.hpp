
#pragma once

#include <glm/matrix.hpp>

namespace Sim::Graphics
{

struct Light
{
	constexpr Light(glm::vec3 pos, glm::vec3 colour) :
			pos(pos), colour(colour)
	{

	}
	
	glm::vec3 pos;
	float padding1;
	glm::vec3 colour;
	float padding2;

} __attribute__((packed));

};

