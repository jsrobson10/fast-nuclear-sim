
#pragma once

#include <GLFW/glfw3.h>

#include "../system.hpp"

namespace sim::graphics::window
{

void create();
bool should_close();
void loop(sim::system& sys);
void destroy();
void close();

GLFWwindow* get_window();

}

