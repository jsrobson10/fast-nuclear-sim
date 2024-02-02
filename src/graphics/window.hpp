
#pragma once

#include <GLFW/glfw3.h>

#include "../system.hpp"

namespace sim::graphics::window
{

bool should_close();
void create();
void update(double dt);
void render();
void destroy();
void close();

GLFWwindow* get_window();

}

