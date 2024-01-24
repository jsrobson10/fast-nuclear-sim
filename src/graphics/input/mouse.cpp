
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "mouse.hpp"
#include "../window.hpp"
#include "../camera.hpp"

using namespace sim::graphics;

static double xpos = 0, ypos = 0;

static void cb_cursor_pos(GLFWwindow* win, double x, double y)
{
	camera::rotate(x - xpos, y - ypos);

	xpos = x;
	ypos = y;
}

void mouse::get(double& x, double& y)
{
	x = xpos;
	y = ypos;
}

void mouse::init()
{
	GLFWwindow* win = window::get_window();
	glfwSetCursorPosCallback(win, cb_cursor_pos);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(win, 0, 0);
}

