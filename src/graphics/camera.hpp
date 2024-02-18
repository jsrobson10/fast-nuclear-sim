
#pragma once

#include <glm/matrix.hpp>
#include <json/json.h>

#include "../system.hpp"

namespace Sim::Graphics::Camera
{

glm::vec<3, double> get_normal();
glm::vec<3, double> get_pos();
glm::mat4 get_matrix();
double get_pitch();
double get_yaw();

Json::Value serialize();
void load(const Json::Value& node);

void init();
void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void update(double dt);

};

