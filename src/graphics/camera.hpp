
#pragma once

#include <glm/matrix.hpp>

#include "../system.hpp"

namespace sim::graphics::camera
{

glm::mat4 get_matrix();
glm::vec<3, double> get_normal();
glm::vec<3, double> get_pos();

void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void update(double dt);

};

