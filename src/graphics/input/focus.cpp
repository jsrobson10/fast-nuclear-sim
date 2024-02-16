
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
#include <vector>

using namespace Sim::Graphics;

static glm::vec<3, double> trigger_near;
static glm::vec<3, double> trigger_far;

static std::vector<std::unique_ptr<Focus::Focus>> stack;
static std::unique_ptr<Focus::Focus> state = nullptr;
static bool mouse_visible = false;
static bool mouse_locked = false;
static bool triggered = false;

void Focus::on_keypress(int key, int sc, int action, int mods)
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

void Focus::on_mouse_button(int button, int action, int mods)
{
	if(state)
	{
		state->on_mouse_button(button, action, mods);
	}

	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if(is_mouse_locked() && mouse_visible)
		{
			double mx, my;
			Mouse::get(mx, my);

			glm::vec2 wsize = Resize::get_size();
			glm::vec4 viewport = glm::vec4(0, 0, wsize);
			glm::vec2 mouse(mx, wsize.y - my);

			trigger_near = glm::unProject(glm::vec3(mouse, -1), Camera::get_matrix(), Window::projection_matrix, viewport);
			trigger_far = glm::unProject(glm::vec3(mouse, 1), Camera::get_matrix(), Window::projection_matrix, viewport);
			triggered = true;
		}

		else if(!mouse_visible)
		{
			trigger_near = Camera::get_pos();
			trigger_far = trigger_near + Camera::get_normal();
			triggered = true;
		}
	}
}

void Focus::on_cursor_pos(double x, double y)
{
	if(state)
	{
		state->on_cursor_pos(x, y);
	}
}

void Focus::on_charcode(unsigned int c)
{
	if(state)
	{
		state->on_charcode(c);
	}
}

glm::vec<3, double> Focus::get_trigger_near()
{
	return trigger_near;
}

glm::vec<3, double> Focus::get_trigger_far()
{
	return trigger_far;
}

void Focus::update(double dt)
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
			Mouse::show_cursor();
		}

		else
		{
			Mouse::hide_cursor();
		}

		mouse_visible = c;
	}

	if(state)
	{
		state->update(dt);
	}
}

void Focus::render_ui()
{
	if(state)
	{
		state->render_ui();
	}
}

void Focus::render()
{
	if(state)
	{
		state->render();
	}
}

bool Focus::is_focused()
{
	return (state != nullptr);
}

void Focus::clear_focus()
{
	state = nullptr;

	if(stack.size() != 0)
	{
		state = std::move(stack.back());
		stack.pop_back();
	}
}

bool Focus::is_mouse_locked()
{
	return is_focused() || mouse_locked;
}

void Focus::clear_mouse_locked()
{
	mouse_locked = false;
	clear_focus();
}

void Focus::set(std::unique_ptr<Focus> f)
{
	if(state != nullptr)
	{
		stack.push_back(std::move(state));
	}
	
	state = std::move(f);
}

bool Focus::is_triggered()
{
	return triggered;
}

