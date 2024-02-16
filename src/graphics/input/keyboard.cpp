
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "focus.hpp"
#include "keyboard.hpp"
#include "../window.hpp"
#include "../resize.hpp"
#include "../camera.hpp"
#include "../../system.hpp"

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

		switch(key)
		{
		case GLFW_KEY_1:
			sim::System::active.speed = 1; // 1 s/s
			break;
		case GLFW_KEY_2:
			sim::System::active.speed = 10; // 10 s/s
			break;
		case GLFW_KEY_3:
			sim::System::active.speed = 60; // 1 min/s
			break;
		case GLFW_KEY_4:
			sim::System::active.speed = 600; // 10 min/s
			break;
		case GLFW_KEY_5:
			sim::System::active.speed = 3600; // 1 h/s
			break;
		case GLFW_KEY_6:
			sim::System::active.speed = 43200; // 12 h/s
			break;
		case GLFW_KEY_O:
			sim::System::save();
			break;
		case GLFW_KEY_L:
			sim::System::load();
			break;
		}
	}

	if(action == GLFW_RELEASE)
	{
		pressed[key] = false;
	}

	focus::on_keypress(key, sc, action, mods);
}

static void cb_charcode(GLFWwindow* win, unsigned int code)
{
	focus::on_charcode(code);
}

bool keyboard::is_pressed(int key)
{
	if(focus::is_mouse_locked())
	{
		return false;
	}
	
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
	glfwSetCharCallback(win, cb_charcode);
}

