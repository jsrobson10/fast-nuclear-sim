
#pragma once

#include <glm/vec2.hpp>

namespace Sim::Graphics::Resize
{

void init();
bool get_fullscreen();
void toggle_fullscreen();
void set_fullscreen(bool fullscreen);
void check_fullscreen();
glm::vec<2, int> get_size();
float get_aspect();

};

