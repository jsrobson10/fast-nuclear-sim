
#pragma once

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include "../system.hpp"

namespace sim::graphics::camera
{

glm::mat4 get_matrix();
void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void update(const system& sys, double dt);

};

