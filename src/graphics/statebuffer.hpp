
#pragma once

#include <glm/matrix.hpp>

namespace Sim::Graphics
{

struct StateBuffer
{
	glm::mat4 camera;
	glm::mat4 projection;

	static void init();
	static void set(const StateBuffer& sb);
};

};

