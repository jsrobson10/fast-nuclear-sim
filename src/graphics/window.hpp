
#pragma once

#include <GLFW/glfw3.h>

namespace sim::graphics::window
{

void create();
bool should_close();
void loop();
void destroy();
void close();

GLFWwindow* get_window();

}

