
#pragma once

#include <glm/vec2.hpp>

namespace Sim::Graphics::Mouse
{

void init();
glm::vec2 get();
glm::vec2 get_normalized();
void get(double& x, double& y);
void show_cursor();
void hide_cursor();

};

