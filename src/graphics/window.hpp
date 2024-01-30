
#pragma once

#include <GLFW/glfw3.h>

#include "../system.hpp"

namespace sim::graphics::window
{

bool should_close();
void create(sim::system& sys);
void loop(sim::system& sys);
void destroy();
void close();

GLFWwindow* get_window();

}

