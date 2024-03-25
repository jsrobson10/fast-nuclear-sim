
#pragma once

#include <glm/vec4.hpp>

namespace Sim::Graphics::Data
{

struct Material
{
	glm::vec4 colour = {1, 1, 1, 1};
	float roughness = 1;
	float metalness = 1;
	float luminance = 1;
	float padding = 0;
};

static_assert(sizeof(Material) == 32);

};

