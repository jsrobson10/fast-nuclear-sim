
#pragma once

#include <glm/matrix.hpp>
#include <json/json.h>

#include "../system.hpp"
#include "data/model.hpp"

namespace Sim::Graphics::Camera
{

glm::vec<3, double> get_normal();
glm::vec<3, double> get_pos();
glm::vec<3, double> get_pos_base();
glm::mat4 get_matrix();
double get_pitch();
double get_yaw();

Json::Value serialize();
void load(const Json::Value& node);

void init(const Data::Model& model);
void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void update(double dt);

};

