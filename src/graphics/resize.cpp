
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "resize.hpp"
#include "window.hpp"

using namespace Sim::Graphics;
using namespace Sim::Graphics::Resize;
using namespace Sim::Graphics::Window;

static bool is_fullscreen = false;

static int win_w = 800;
static int win_h = 600;

static int win_restore_w;
static int win_restore_h;
static int win_restore_x;
static int win_restore_y;

glm::vec<2, int> Resize::get_size()
{
	return {win_w, win_h};
}

float Resize::get_aspect()
{
	return (float)win_w / (float)win_h;
}

void Resize::toggle_fullscreen()
{
	set_fullscreen(!is_fullscreen);
}

bool Resize::get_fullscreen()
{
	return is_fullscreen;
}

void Resize::set_fullscreen(bool fullscreen)
{
	if(fullscreen == is_fullscreen)
	{
		return;
	}

	GLFWwindow* win = Window::get_window();
	is_fullscreen = fullscreen;

	if(fullscreen)
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
	Window::reload_rbos();
}

void Resize::init()
{
	GLFWwindow* win = Window::get_window();
	glfwSetFramebufferSizeCallback(win, cb_framebuffer_size);
}

