
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "keyboard.hpp"
#include "window.hpp"
#include "resize.hpp"

using namespace sim::graphics;

static void cb_keypress(GLFWwindow* win, int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_F11 && action == GLFW_RELEASE)
	{
		resize::toggle_fullscreen();
	}
}

void keyboard::init()
{
	GLFWwindow* win = window::get_window();
	glfwSetKeyCallback(win, cb_keypress);
}

