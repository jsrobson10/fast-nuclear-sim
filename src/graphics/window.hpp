
#pragma once

#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>

#include "../system.hpp"

namespace Sim::Graphics::Window
{

extern glm::mat4 projection_matrix;

bool should_close();
void create();
void reload();
void update(double dt);
void render();
void render_scene();
void render_player();
void destroy();
void close();

GLFWwindow* get_window();

}

