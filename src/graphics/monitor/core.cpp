
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "../input/focus.hpp"
#include "../data/arrays.hpp"
#include "../data/texture.hpp"
#include "../../system.hpp"
#include "../../util/streams.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <iostream>
#include <sstream>

using namespace Sim;
using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;
using namespace Sim::Graphics::Data;
using namespace Sim::Util::Streams;

static void set_all(bool state)
{
	for(int i = 0; i < System::active->reactor.rods.size(); i++)
	{
		Reactor::Rod* r = System::active->reactor.rods[i].get();

		if(r->should_select())
		{
			r->selected = state;
		}
	}
}

struct CoreMonitor : public Focus::FocusType
{
	Core* parent;

	CoreMonitor(Core* parent)
	{
		this->parent = parent;
	}
	
	virtual void on_keypress(int key, int sc, int action, int mods)
	{
		if(action != GLFW_PRESS)
		{
			return;
		}
	
		Sim::System& sys = *System::active;

		switch(key)
		{
		case GLFW_KEY_KP_7:
			set_all(true);
			break;
		case GLFW_KEY_KP_8:
			sys.reactor.move_cursor(-sys.reactor.height);
			break;
		case GLFW_KEY_KP_9:
			set_all(false);
			break;
		case GLFW_KEY_KP_4:
			sys.reactor.move_cursor(-1);
			break;
		case GLFW_KEY_KP_5:
			sys.reactor.move_cursor(sys.reactor.height);
			break;
		case GLFW_KEY_KP_6:
			sys.reactor.move_cursor(1);
			break;
		case GLFW_KEY_KP_1:
			sys.reactor.reset_rod_speed();
			break;
		case GLFW_KEY_KP_2:
			sys.reactor.toggle_selected();
			break;
		default:
			return;
		}
	}
};

struct CoreJoystick : public Focus::FocusType
{
	Core* parent;

	CoreJoystick(Core* parent)
	{
		this->parent = parent;
	}
	
	virtual void on_cursor_pos(double x, double y)
	{
		System::active->reactor.add_rod_speed(y * 1e-6);
	}

	virtual ~CoreJoystick()
	{
		System::active->reactor.reset_rod_speed();
	}

	virtual void on_mouse_button(int button, int action, int mods)
	{
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			Focus::clear_focus();
		}
	}

	virtual bool cursor_is_visible()
	{
		return false;
	}
};

Core::Core(const Model& model) : Data::MeshGen("core")
{
	mat = model.load_matrix("translation_monitor_3");
	m_buttons[0] = model.load("click_reactor_numpad_1");
	m_buttons[1] = model.load("click_reactor_numpad_2");
	m_buttons[2] = model.load("click_reactor_numpad_3");
	m_buttons[3] = model.load("click_reactor_numpad_4");
	m_buttons[4] = model.load("click_reactor_numpad_5");
	m_buttons[5] = model.load("click_reactor_numpad_6");
	m_buttons[6] = model.load("click_reactor_numpad_7");
	m_buttons[7] = model.load("click_reactor_numpad_8");
	m_buttons[8] = model.load("click_reactor_numpad_9");
	m_joystick = model.load("click_reactor_joystick");
	m_monitor = model.load("translation_monitor_3");
	m_scram = model.load("click_scram");
}

void Core::update(double dt)
{
	Sim::System& sys = *System::active;
	
	if(m_monitor.check_focus())
		Focus::set(std::make_unique<CoreMonitor>(this));
	if(m_joystick.check_focus())
		Focus::set(std::make_unique<CoreJoystick>(this));
	if(m_scram.check_focus())
		sys.reactor.scram();
	if(m_buttons[0].check_focus())
		set_all(true);
	if(m_buttons[1].check_focus())
		sys.reactor.move_cursor(-sys.reactor.height);
	if(m_buttons[2].check_focus())
		set_all(false);
	if(m_buttons[3].check_focus())
		sys.reactor.move_cursor(-1);
	if(m_buttons[4].check_focus())
		sys.reactor.move_cursor(sys.reactor.height);
	if(m_buttons[5].check_focus())
		sys.reactor.move_cursor(1);
	if(m_buttons[6].check_focus())
		sys.reactor.reset_rod_speed();
	if(m_buttons[7].check_focus())
		sys.reactor.toggle_selected();
}

void Core::remesh_static(Mesh& rmesh)
{
	Sim::System& sys = *System::active;
	Data::Fonts::BASE.load_text(rmesh, "Reactor Core", {.size=0.04, .mat=mat, .bake=true});
	
	int colour_id = 0;
	double step = sys.reactor.cell_width / sys.vessel.diameter * 0.8;
	double sx = 0.5 - (sys.reactor.width - 1) * step / 2.0;
	double sy = 0.5 - (sys.reactor.height - 1) * step / 2.0;
	
	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(step * 0.4, step * 0.4, 1));
	glm::mat4 mat_select = glm::translate(glm::mat4(1), glm::vec3(-0.8, -0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));
	glm::mat4 mat_spec = glm::translate(glm::mat4(1), glm::vec3(0.8, -0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));

	Data::Mesh::Primitive<4, 6> prim {
		.vertex_base = {.material={0, 0, 1}},
		.indices = {0, 1, 3, 0, 3, 2},
		.vertices = {
			{.texpos={0, 0}, .pos=glm::vec3(glm::vec4(-0.75, -0.75, 0, 1))}, 
			{.texpos={0, 1}, .pos=glm::vec3(glm::vec4(-0.75,  0.75, 0, 1))}, 
			{.texpos={1, 0}, .pos=glm::vec3(glm::vec4( 0.75, -0.75, 0, 1))}, 
			{.texpos={1, 1}, .pos=glm::vec3(glm::vec4( 0.75,  0.75, 0, 1))}, 
		},
	};
	
	prim.vertex_base.colour = {1, 0, 0, 1};
	rmesh.add(prim, mat);
	
	prim.vertex_base.colour = {1, 1, 1, 1};
	
	for(int i = 0; i < sys.reactor.size; i++)
	{
		int x = i % sys.reactor.width;
		int y = i / sys.reactor.width;
		double ox = sx + x * step;
		double oy = sy + y * step;

		Reactor::Rod* r = sys.reactor.rods[i].get();

		if(!r->should_display())
		{
			continue;
		}

		glm::mat4 mat = this->mat * glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale;
		glm::vec4 colour_heat = r->get_heat_colour();
		glm::vec4 colour_spec = r->get_colour();

		if(colour_heat[3] == 0)
		{
			continue;
		}

		prim.vertex_base.colour_id = colour_id++;
		rmesh.add(prim, mat, true);
		
		prim.vertex_base.colour_id = colour_id++;
		rmesh.add(prim, mat * mat_select, true);
		
		if(colour_spec[3] != 0)
		{
			prim.vertex_base.colour_id = colour_id++;
			rmesh.add(prim, mat * mat_spec, true);
		}
	}

	rmesh.colour_ids += colour_id;
}

void Core::get_static_transforms(std::vector<glm::mat4>& transforms)
{
	Sim::System& sys = *System::active;
	double step = sys.reactor.cell_width / sys.vessel.diameter * 0.8;
	double sx = 0.5 - (sys.reactor.width - 1) * step / 2.0;
	double sy = 0.5 - (sys.reactor.height - 1) * step / 2.0;

	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(step * 0.4, step * 0.4, 1));
	glm::mat4 mat_cursor = glm::translate(glm::mat4(1), glm::vec3(-0.8, 0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));

	if(sys.reactor.cursor >= 0 && sys.reactor.cursor < sys.reactor.size)
	{
		int x = sys.reactor.cursor % sys.reactor.width;
		int y = sys.reactor.cursor / sys.reactor.width;
		double ox = sx + x * step;
		double oy = sy + y * step;

		transforms.push_back(glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale * mat_cursor);
	}

	else
	{
		transforms.push_back(glm::mat4(0));
	}
}

void Core::get_static_colours(std::vector<glm::vec4>& colours)
{
	Sim::System& sys = *System::active;

	for(int i = 0; i < sys.reactor.size; i++)
	{
		Reactor::Rod* r = sys.reactor.rods[i].get();
		glm::vec4 spec = r->get_colour();

		if(!r->should_display())
		{
			continue;
		}

		colours.push_back(r->get_heat_colour());
		colours.push_back(r->selected ? glm::vec4{1, 1, 0, 1} : glm::vec4{1, 1, 0, 0.25});

		if(spec[3] != 0)
		{
			colours.push_back(spec);
		}
	}
}

