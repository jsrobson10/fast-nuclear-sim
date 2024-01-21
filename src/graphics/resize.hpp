
#pragma once

#include <glm/vec2.hpp>

namespace sim::graphics::resize
{

void init();
void toggle_fullscreen();
glm::vec2 get_size();
double get_aspect();

};

