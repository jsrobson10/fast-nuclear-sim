
#pragma once

#include <glm/matrix.hpp>

namespace Sim::Graphics::Data
{

struct Camera
{
	glm::vec3 pos;
	glm::vec3 look;
	glm::vec3 up;
	float fov;

	float pitch = 0;
	float yaw = 0;
	float zoom = 1;
};

};

