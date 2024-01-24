
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "keyboard.hpp"
#include "../window.hpp"
#include "../resize.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

static std::unordered_map<int, bool> pressed;

static void cb_keypress(GLFWwindow* win, int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		resize::toggle_fullscreen();
	}

	if(action == GLFW_PRESS)
	{
		pressed[key] = true;
	}

	if(action == GLFW_RELEASE)
	{
		pressed[key] = false;
	}
}

bool keyboard::is_pressed(int key)
{
	auto it = pressed.find(key);

	if(it == pressed.end())
	{
		return false;
	}

	else
	{
		return it->second;
	}
}

void keyboard::init()
{
	GLFWwindow* win = window::get_window();
	glfwSetKeyCallback(win, cb_keypress);
}

