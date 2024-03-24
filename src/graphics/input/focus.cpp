
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "focus.hpp"
#include "../../util/math.hpp"
#include "../window.hpp"
#include "../camera.hpp"
#include "../resize.hpp"
#include "../menu/menu.hpp"
#include "mouse.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

using namespace Sim::Graphics;

static glm::vec<3, double> trigger_near;
static glm::vec<3, double> trigger_far;

static std::vector<std::unique_ptr<Focus::FocusType>> stack;
static std::unique_ptr<Focus::FocusType> state = nullptr;
static bool mouse_visible = false;
static bool triggered = false;
static bool triggered_release = false;

void Focus::on_keypress(int key, int sc, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		if(is_focused())
		{
			clear_focus();
		}

		else
		{
			Menu::open_pause();
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

	if(button == GLFW_MOUSE_BUTTON_LEFT && (action == GLFW_RELEASE || action == GLFW_PRESS))
	{
		bool t = false;

		if(is_focused() && mouse_visible)
		{
			double mx, my;
			Mouse::get(mx, my);

			glm::vec2 wsize = Resize::get_size();
			glm::vec4 viewport = glm::vec4(0, 0, wsize);
			glm::vec2 mouse(mx, wsize.y - my);

			trigger_near = glm::unProject(glm::vec3(mouse, -1), Camera::get_matrix(), Window::projection_matrix, viewport);
			trigger_far = glm::unProject(glm::vec3(mouse, 1), Camera::get_matrix(), Window::projection_matrix, viewport);
			t = true;
		}

		else if(!mouse_visible)
		{
			trigger_near = Camera::get_pos();
			trigger_far = trigger_near + Camera::get_normal();
			t = true;
		}

		if(t)
		{
			t = (action == GLFW_PRESS);
			triggered_release = !t;
			triggered = t;
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
	bool c = is_focused();

	if(state && !state->cursor_is_visible)
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
	
	triggered = false;
	triggered_release = false;
}

void Focus::remesh_ui(Data::Mesh& rmesh)
{
	if(state)
	{
		state->remesh_ui(rmesh);
	}
}

void Focus::remesh(Data::Mesh& rmesh)
{
	if(state)
	{
		state->remesh(rmesh);
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

void Focus::set(std::unique_ptr<FocusType> f)
{
	if(state != nullptr && state->will_restore_later)
	{
		stack.push_back(std::move(state));
	}
	
	state = std::move(f);
}

bool Focus::is_triggered(Trigger level)
{
	if(state && state->min_trigger_level > level)
	{
		return false;
	}

	return triggered;
}

bool Focus::is_triggered_release()
{
	return triggered_release;
}

bool Focus::should_advance_time()
{
	return state ? state->will_advance_time : true;
}

