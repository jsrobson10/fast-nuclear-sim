
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "focus.hpp"
#include "mouse.hpp"
#include "../window.hpp"
#include "../camera.hpp"
#include "../resize.hpp"

#include <memory>

using namespace Sim::Graphics;

static double xpos = 0, ypos = 0;

struct FocusMouse : public Focus::FocusType
{
	FocusMouse()
	{
		cursor_is_visible = true;
		min_trigger_level = Focus::Trigger::NONE;
	}

	void on_mouse_button(int button, int action, int mods) override
	{
		if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			Focus::clear_focus();
		}
	}
};

static void cb_cursor_pos(GLFWwindow* win, double x, double y)
{
	if(Focus::is_focused())
	{
		Focus::on_cursor_pos(x - xpos, y - ypos);
	}
	
	else
	{
		Camera::rotate(x - xpos, y - ypos);
	}

	xpos = x;
	ypos = y;
}

void cb_mouse_button(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !Focus::is_focused())
	{
		return Focus::set(std::make_unique<FocusMouse>());
	}
	
	Focus::on_mouse_button(button, action, mods);
}

void Mouse::get(double& x, double& y)
{
	x = xpos;
	y = ypos;
}

glm::vec2 Mouse::get()
{
	return {xpos, ypos};
}

glm::vec2 Mouse::get_normalized()
{
	glm::vec2 pos(xpos, ypos);
	glm::vec2 size = Resize::get_size();

	return glm::vec2(
		(pos.x / size.x * 2 - 1) * size.x / size.y,
		pos.y / size.y * 2 - 1
	);
}

void Mouse::show_cursor()
{
	double x, y;
	GLFWwindow* win = Window::get_window();
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwGetCursorPos(win, &x, &y);
	cb_cursor_pos(win, x, y);
}

void Mouse::hide_cursor()
{
	double x, y;
	GLFWwindow* win = Window::get_window();
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(win, &x, &y);
	cb_cursor_pos(win, x, y);
}

void Mouse::init()
{
	GLFWwindow* win = Window::get_window();
	glfwSetCursorPosCallback(win, cb_cursor_pos);
	glfwSetMouseButtonCallback(win, cb_mouse_button);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(win, 0, 0);
}

