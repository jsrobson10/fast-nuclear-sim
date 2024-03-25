
#pragma once

#include <glm/matrix.hpp>
#include <string>

namespace Sim::Graphics::Data
{

struct Camera
{
	std::string name;
	glm::vec3 pos;
	glm::vec3 look;
	glm::vec3 up;
	float fov;

	float pitch = 0;
	float yaw = 0;
	float zoom = 1;
};

};

