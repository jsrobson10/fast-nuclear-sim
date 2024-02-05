
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "focus.hpp"
#include "mouse.hpp"
#include "../window.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

static double xpos = 0, ypos = 0;

static void cb_cursor_pos(GLFWwindow* win, double x, double y)
{
	if(focus::is_mouse_locked())
	{
		focus::on_cursor_pos(x - xpos, y - ypos);
	}
	
	else
	{
		camera::rotate(x - xpos, y - ypos);
	}

	xpos = x;
	ypos = y;
}

void cb_mouse_button(GLFWwindow* window, int button, int action, int mods)
{
	focus::on_mouse_button(button, action, mods);
}

void mouse::get(double& x, double& y)
{
	x = xpos;
	y = ypos;
}

glm::vec2 mouse::get()
{
	return {xpos, ypos};
}

void mouse::show_cursor()
{
	double x, y;
	GLFWwindow* win = window::get_window();
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwGetCursorPos(win, &x, &y);
	cb_cursor_pos(win, x, y);
}

void mouse::hide_cursor()
{
	double x, y;
	GLFWwindow* win = window::get_window();
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(win, &x, &y);
	cb_cursor_pos(win, x, y);
}

void mouse::init()
{
	GLFWwindow* win = window::get_window();
	glfwSetCursorPosCallback(win, cb_cursor_pos);
	glfwSetMouseButtonCallback(win, cb_mouse_button);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(win, 0, 0);
}
