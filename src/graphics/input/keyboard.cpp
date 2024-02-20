
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "focus.hpp"
#include "keyboard.hpp"
#include "../window.hpp"
#include "../resize.hpp"
#include "../camera.hpp"
#include "../../system.hpp"

using namespace Sim::Graphics;

static std::unordered_map<int, bool> pressed;

static void cb_keypress(GLFWwindow* win, int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		Resize::toggle_fullscreen();
	}

	if(action == GLFW_PRESS)
	{
		pressed[key] = true;

		switch(key)
		{
		case GLFW_KEY_1:
			Sim::System::active->speed = 1; // 1 s/s
			break;
		case GLFW_KEY_2:
			Sim::System::active->speed = 10; // 10 s/s
			break;
		case GLFW_KEY_3:
			Sim::System::active->speed = 60; // 1 min/s
			break;
		case GLFW_KEY_4:
			Sim::System::active->speed = 600; // 10 min/s
			break;
		case GLFW_KEY_5:
			Sim::System::active->speed = 3600; // 1 h/s
			break;
		case GLFW_KEY_O:
			Sim::System::save();
			break;
		case GLFW_KEY_L:
			Sim::System::load();
			break;
		}
	}

	if(action == GLFW_RELEASE)
	{
		pressed[key] = false;
	}

	Focus::on_keypress(key, sc, action, mods);
}

static void cb_charcode(GLFWwindow* win, unsigned int code)
{
	Focus::on_charcode(code);
}

bool Keyboard::is_pressed(int key)
{
	if(Focus::is_mouse_locked())
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

void Keyboard::init()
{
	GLFWwindow* win = Window::get_window();
	glfwSetKeyCallback(win, cb_keypress);
	glfwSetCharCallback(win, cb_charcode);
}

