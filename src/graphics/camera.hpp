
#pragma once

#include <glm/matrix.hpp>

namespace sim::graphics::camera
{

glm::mat4 get();
void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void update();

};

