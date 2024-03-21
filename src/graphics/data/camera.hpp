
#pragma once

#include <glm/matrix.hpp>
#include <string>

namespace Sim::Graphics::Data
{

struct Camera
{
	const std::string name;
	const glm::vec3 pos;
	const glm::vec3 look;
	const glm::vec3 up;
	const float fov;

	float pitch = 0;
	float yaw = 0;
	float zoom = 1;
};

};

