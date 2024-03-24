
#pragma once

#include <glm/matrix.hpp>

#include "../system.hpp"

struct GLFWwindow;

namespace Sim::Graphics::Window
{

extern glm::mat4 projection_matrix;

bool should_close();
void create();
void reload();
void update(double dt);
void reload_rbos();
void render();
void bind_scene_ssbo();
void render_scene();
void render_dynamic();
void render_player();
void destroy();
void close();

GLFWwindow* get_window();

}

