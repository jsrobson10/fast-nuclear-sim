
#include "focus.hpp"
#include "../camera.hpp"
#include "../../util/math.hpp"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace sim::graphics;

static std::unique_ptr<focus::focus_t> state = nullptr;
static bool triggered = false;

void focus::on_keypress(int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		state = nullptr;
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

bool focus::is_focused()
{
	return (state != nullptr);
}

void focus::clear()
{
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

void focus::clear_trigger()
{
	triggered = false;
}

