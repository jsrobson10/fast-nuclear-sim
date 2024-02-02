
#pragma once

#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>

#include "../system.hpp"

namespace sim::graphics::window
{

extern glm::mat4 projection_matrix;

bool should_close();
void create();
void update(double dt);
void render();
void destroy();
void close();

GLFWwindow* get_window();

}

