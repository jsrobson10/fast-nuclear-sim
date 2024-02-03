
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "helpers.hpp"
#include "../locations.hpp"
#include "../input/focus.hpp"
#include "../mesh/arrays.hpp"
#include "../../system.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <iostream>
#include <sstream>

using namespace sim::graphics;
using namespace sim::graphics::monitor;

static void set_all(bool state)
{
	for(int i = 0; i < sim::system::active.reactor->rods.size(); i++)
	{
		sim::reactor::rod* r = sim::system::active.reactor->rods[i].get();

		if(r->should_select())
		{
			r->selected = state;
		}
	}
}

struct core_monitor : public focus::focus_t
{
	virtual void on_keypress(int key, int sc, int action, int mods)
	{
		if(action != GLFW_PRESS)
		{
			return;
		}
	
		sim::system& sys = sim::system::active;

		switch(key)
		{
		case GLFW_KEY_KP_7:
			set_all(true);
			break;
		case GLFW_KEY_KP_8:
			sys.reactor->move_cursor(-sys.reactor->height);
			break;
		case GLFW_KEY_KP_9:
			set_all(false);
			break;
		case GLFW_KEY_KP_4:
			sys.reactor->move_cursor(-1);
			break;
		case GLFW_KEY_KP_5:
			sys.reactor->toggle_selected();
			break;
		case GLFW_KEY_KP_6:
			sys.reactor->move_cursor(1);
			break;
		case GLFW_KEY_KP_1:
			sys.reactor->reset_rod_speed();
			break;
		case GLFW_KEY_KP_2:
			sys.reactor->move_cursor(sys.reactor->height);
			break;
		}
	}
};

struct core_joystick : public focus::focus_t
{
	virtual void on_cursor_pos(double x, double y)
	{
		sim::system::active.reactor->add_rod_speed(y * 1e-6);
	}

	virtual void on_mouse_button(int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			focus::clear_focus();
		}
	}

	virtual bool cursor_is_visible()
	{
		return false;
	}
};

core::core()
{
}

void core::init()
{
	mesh1.model_matrix = locations::monitors[2];
	mesh1.colour_matrix = arrays::colour({1, 1, 1, 1});
	
	sim::graphics::mesh rmesh1, rmesh2;
	
	rmesh1.load_text("Reactor Core", 0.04);
	rmesh2.load_model("../assets/model/", "reactor_core_interface_circle.stl");
	rmesh1.add(rmesh2, glm::mat4(1));

	mesh1.bind();
	mesh1.set(rmesh1, GL_STATIC_DRAW);
	rmesh2.load_model("../assets/model/", "reactor_core_interface_cell.stl");
	mesh2.bind();
	mesh2.set(rmesh2, GL_STATIC_DRAW);

	m_buttons[0].load_model("../assets/model/", "reactor_core_button1.stl");
	m_buttons[1].load_model("../assets/model/", "reactor_core_button2.stl");
	m_buttons[2].load_model("../assets/model/", "reactor_core_button3.stl");
	m_buttons[3].load_model("../assets/model/", "reactor_core_button4.stl");
	m_buttons[4].load_model("../assets/model/", "reactor_core_button5.stl");
	m_buttons[5].load_model("../assets/model/", "reactor_core_button6.stl");
	m_buttons[6].load_model("../assets/model/", "reactor_core_button7.stl");
	m_buttons[7].load_model("../assets/model/", "reactor_core_button8.stl");
	m_buttons[8].load_model("../assets/model/", "reactor_core_button9.stl");
	m_joystick.load_model("../assets/model/", "reactor_core_joystick.stl");
	m_monitor.load_model("../assets/model/", "reactor_core_input.stl");
	m_scram.load_model("../assets/model/", "reactor_core_scram.stl");
}

void core::update()
{
	sim::system& sys = sim::system::active;
	
	if(m_monitor.check_focus())
		focus::set(std::make_unique<core_monitor>());
	if(m_joystick.check_focus())
		focus::set(std::make_unique<core_joystick>());
	if(m_scram.check_focus())
		sys.reactor->scram();
	if(m_buttons[0].check_focus())
		set_all(true);
	if(m_buttons[1].check_focus())
		sys.reactor->move_cursor(-sys.reactor->height);
	if(m_buttons[2].check_focus())
		set_all(false);
	if(m_buttons[3].check_focus())
		sys.reactor->move_cursor(-1);
	if(m_buttons[4].check_focus())
		sys.reactor->toggle_selected();
	if(m_buttons[5].check_focus())
		sys.reactor->move_cursor(1);
	if(m_buttons[6].check_focus())
		sys.reactor->reset_rod_speed();
	if(m_buttons[7].check_focus())
		sys.reactor->move_cursor(sys.reactor->height);
}

void core::render()
{
	sim::system& sys = sim::system::active;
	
	double step = 1 / (sys.vessel->diameter / sys.reactor->cell_width * 0.8);
	double sx = 0.5 - (sys.reactor->width - 1) * step / 2.0;
	double sy = 0.5 - (sys.reactor->height - 1) * step / 2.0;

	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(step * 0.4, step * 0.4, 1));
	glm::mat4 mat_select = glm::translate(glm::mat4(1), glm::vec3(-0.8, -0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 1));
	glm::mat4 mat_cursor = glm::translate(glm::mat4(1), glm::vec3(-0.8, 0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.5, 0.5, 1));

	mesh1.bind();
	mesh1.uniform();
	mesh1.render();
	mesh2.bind();

	for(int i = 0; i < sys.reactor->size; i++)
	{
		int x = i % sys.reactor->width;
		int y = i / sys.reactor->width;
		double ox = sx + x * step;
		double oy = sy + y * step;

		reactor::rod* r = sys.reactor->rods[i].get();
		glm::vec4 colour = r->get_colour();

		if(colour[3] == 0)
		{
			continue;
		}

		glm::mat4 mat = mesh1.model_matrix * glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale;

		mesh2.model_matrix = mat;
		mesh2.colour_matrix = arrays::colour(colour);
		mesh2.uniform();
		mesh2.render();

		if(sys.reactor->cursor == i)
		{
			mesh2.model_matrix = mat * mat_cursor;
			mesh2.colour_matrix = arrays::colour({1, 0, 0, 1});
			mesh2.uniform();
			mesh2.render();
		}

		if(r->selected)
		{
			mesh2.model_matrix = mat * mat_select;
			mesh2.colour_matrix = arrays::colour({1, 1, 0, 1});
			mesh2.uniform();
			mesh2.render();
		}
	}
}

