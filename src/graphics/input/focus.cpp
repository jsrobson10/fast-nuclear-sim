
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "focus.hpp"
#include "../../util/math.hpp"
#include "../window.hpp"
#include "../camera.hpp"
#include "../resize.hpp"
#include "mouse.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

using namespace sim::graphics;

static glm::vec<3, double> trigger_near;
static glm::vec<3, double> trigger_far;

static std::unique_ptr<focus::focus_t> state = nullptr;
static bool mouse_visible = false;
static bool mouse_locked = false;
static bool triggered = false;

void focus::on_keypress(int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if(is_mouse_locked())
		{
			clear_mouse_locked();
		}

		else
		{
			mouse_locked = true;
		}
	}

	if(state)
	{
		state->on_keypress(key, sc, action, mods);
	}
}

void focus::on_mouse_button(int button, int action, int mods)
{
	if(state)
	{
		state->on_mouse_button(button, action, mods);
	}

	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		triggered = true;

		if(is_mouse_locked())
		{
			double mx, my;
			mouse::get(mx, my);

			glm::vec2 wsize = resize::get_size();
			glm::vec4 viewport = glm::vec4(0, 0, wsize);
			glm::vec2 mouse(mx, wsize.y - my);

			trigger_near = glm::unProject(glm::vec3(mouse, -1), camera::get_matrix(), window::projection_matrix, viewport);
			trigger_far = glm::unProject(glm::vec3(mouse, 1), camera::get_matrix(), window::projection_matrix, viewport);
		}

		else
		{
			trigger_near = camera::get_pos();
			trigger_far = trigger_near + camera::get_normal();
		}
	}
}

void focus::on_cursor_pos(double x, double y)
{
	if(state)
	{
		state->on_cursor_pos(x, y);
	}
}

void focus::on_charcode(unsigned int c)
{
	if(state)
	{
		state->on_charcode(c);
	}
}

glm::vec<3, double> focus::get_trigger_near()
{
	return trigger_near;
}

glm::vec<3, double> focus::get_trigger_far()
{
	return trigger_far;
}

void focus::update()
{
	triggered = false;

	bool c = is_mouse_locked();

	if(state && !state->cursor_is_visible())
	{
		c = false;
	}

	if(c != mouse_visible)
	{
		if(c)
		{
			mouse::show_cursor();
		}

		else
		{
			mouse::hide_cursor();
		}

		mouse_visible = c;
	}

	if(state)
	{
		state->update();
	}
}

bool focus::is_focused()
{
	return (state != nullptr);
}

void focus::clear_focus()
{
	state = nullptr;
}

bool focus::is_mouse_locked()
{
	return is_focused() || mouse_locked;
}

void focus::clear_mouse_locked()
{
	mouse_locked = false;
	state = nullptr;
}

void focus::set(std::unique_ptr<focus_t> f)
{
	state = std::move(f);
}

bool focus::is_triggered()
{
	return triggered;
}

