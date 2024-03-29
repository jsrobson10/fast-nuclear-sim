
#pragma once

#include <glm/matrix.hpp>

namespace Sim::Graphics::Data
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
	
	constexpr bool operator==(const Light&) const = default;

};

static_assert(sizeof(Light) == 32);

};

