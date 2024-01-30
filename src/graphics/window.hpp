
#pragma once

#include <GLFW/glfw3.h>

#include "../system.hpp"

namespace sim::graphics::window
{

bool should_close();
void create();
void loop();
void destroy();
void close();

GLFWwindow* get_window();

}

