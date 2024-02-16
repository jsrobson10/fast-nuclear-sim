
#pragma once

#include <glm/vec2.hpp>

namespace Sim::Graphics::Resize
{

void init();
void toggle_fullscreen();
glm::vec<2, int> get_size();
float get_aspect();

};

