
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "resize.hpp"
#include "window.hpp"

using namespace sim::graphics;

static bool is_fullscreen = false;

static int win_w = 800;
static int win_h = 600;

static int win_restore_w;
static int win_restore_h;
static int win_restore_x;
static int win_restore_y;

glm::vec<2, int> resize::get_size()
{
	return {win_w, win_h};
}

float resize::get_aspect()
{
	return (float)win_w / (float)win_h;
}

void resize::toggle_fullscreen()
{
	GLFWwindow* win = window::get_window();
	is_fullscreen = !is_fullscreen;

	if(is_fullscreen)
	{
		win_restore_w = win_w;
		win_restore_h = win_h;

		glfwGetWindowPos(win, &win_restore_x, &win_restore_y);
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
	}

	else
	{
		glfwSetWindowMonitor(win, nullptr, win_restore_x, win_restore_y, win_restore_w, win_restore_h, 0);
	}
}

static void cb_framebuffer_size(GLFWwindow* win, int w, int h)
{
	win_w = w;
	win_h = h;

	glViewport(0, 0, w, h);
}

void resize::init()
{
	GLFWwindow* win = window::get_window();
	glfwSetFramebufferSizeCallback(win, cb_framebuffer_size);
}

