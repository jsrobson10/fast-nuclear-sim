
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
glm::vec<3, double> get_velocity();
glm::mat4 get_matrix();
double get_pitch();
double get_yaw();
float get_fov();

Json::Value serialize();
void load(const Json::Value& node);

void init(Data::Model& model);
void rotate(double pitch, double yaw);
void move(double x, double y, double z);
void set_pos(glm::vec<3, double> pos);
void set_in_main_menu(bool b);
bool is_in_main_menu();
void update(double dt);
void reset();

};

