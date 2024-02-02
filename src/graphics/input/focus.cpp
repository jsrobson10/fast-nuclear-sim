
#include "focus.hpp"
#include "../camera.hpp"
#include "../../util/math.hpp"
#include "mouse.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace sim::graphics;

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

	else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		triggered = true;
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

void focus::update()
{
	triggered = false;

	bool locked = is_mouse_locked();

	if(locked != mouse_visible)
	{
		if(locked)
		{
			mouse::show_cursor();
		}

		else
		{
			mouse::hide_cursor();
		}

		mouse_visible = locked;
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

