
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "../locations.hpp"
#include "../input/focus.hpp"
#include "../mesh/arrays.hpp"
#include "../mesh/texture.hpp"
#include "../../system.hpp"
#include "../../util/streams.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <iostream>
#include <sstream>

using namespace Sim;
using namespace Sim::Graphics;
using namespace Sim::Graphics::Monitor;
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
			sys.reactor.toggle_selected();
			break;
		case GLFW_KEY_KP_6:
			sys.reactor.move_cursor(1);
			break;
		case GLFW_KEY_KP_1:
			sys.reactor.reset_rod_speed();
			break;
		case GLFW_KEY_KP_2:
			sys.reactor.move_cursor(sys.reactor.height);
			break;
		default:
			return;
		}

		parent->is_dirty = true;
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
		parent->is_dirty = true;
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

Core::Core()
{
}

void Core::init()
{
	mesh1.model_matrix = mesh2.model_matrix = Locations::monitors[2];
	mesh1.colour_matrix = mesh2.colour_matrix = Arrays::colour({1, 1, 1, 1});
	
	Sim::Graphics::Mesh rmesh;
	
	rmesh.load_text("Reactor Core", 0.04);
	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);

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

static Mesh add_dot(glm::mat4 model_mat, glm::vec4 colour)
{
	unsigned int indices[] = {0, 1, 3, 0, 3, 2};
	Arrays::Vertex vertices[] = {
		{Texture::handle_white, {0, 0}, model_mat * glm::vec4(-0.75, -0.75, 0, 1), {0, 0, -1}, colour}, 
		{Texture::handle_white, {0, 1}, model_mat * glm::vec4(-0.75,  0.75, 0, 1), {0, 0, -1}, colour}, 
		{Texture::handle_white, {1, 0}, model_mat * glm::vec4( 0.75, -0.75, 0, 1), {0, 0, -1}, colour}, 
		{Texture::handle_white, {1, 1}, model_mat * glm::vec4( 0.75,  0.75, 0, 1), {0, 0, -1}, colour}, 
	};

	Mesh mesh;
	mesh.set_indices(indices, 6);
	mesh.set_vertices(vertices, 4);
	return mesh;
}

void Core::update(double dt)
{
	Sim::System& sys = *System::active;
	
	if(m_monitor.check_focus()) {
		Focus::set(std::make_unique<CoreMonitor>(this));
	}
	if(m_joystick.check_focus()) {
		Focus::set(std::make_unique<CoreJoystick>(this));
	}
	if(m_scram.check_focus()) {
		sys.reactor.scram();
		is_dirty = true;
	}
	if(m_buttons[0].check_focus()) {
		set_all(true);
		is_dirty = true;
	}
	if(m_buttons[1].check_focus()) {
		sys.reactor.move_cursor(-sys.reactor.height);
		is_dirty = true;
	}
	if(m_buttons[2].check_focus()) {
		set_all(false);
		is_dirty = true;
	}
	if(m_buttons[3].check_focus()) {
		sys.reactor.move_cursor(-1);
		is_dirty = true;
	}
	if(m_buttons[4].check_focus()) {
		sys.reactor.toggle_selected();
		is_dirty = true;
	}
	if(m_buttons[5].check_focus()) {
		sys.reactor.move_cursor(1);
		is_dirty = true;
	}
	if(m_buttons[6].check_focus()) {
		sys.reactor.reset_rod_speed();
		is_dirty = true;
	}
	if(m_buttons[7].check_focus()) {
		sys.reactor.move_cursor(sys.reactor.height);
		is_dirty = true;
	}

	clock_now += dt;

	if(clock_at + 1.0/30.0 > clock_now)
	{
		if(!is_dirty)
		{
			return;
		}
	}

	else
	{
		clock_at += 1.0/30.0;
	}

	Sim::Graphics::Mesh rmesh;
	is_dirty = false;
	
	double step = 1 / (sys.vessel.diameter / sys.reactor.cell_width * 0.8);
	double sx = 0.5 - (sys.reactor.width - 1) * step / 2.0;
	double sy = 0.5 - (sys.reactor.height - 1) * step / 2.0;

	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(step * 0.4, step * 0.4, 1));
	glm::mat4 mat_select = glm::translate(glm::mat4(1), glm::vec3(-0.8, -0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));
	glm::mat4 mat_cursor = glm::translate(glm::mat4(1), glm::vec3(-0.8, 0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));
	glm::mat4 mat_spec = glm::translate(glm::mat4(1), glm::vec3(0.8, -0.8, -0.001)) * glm::scale(glm::mat4(1), glm::vec3(0.25, 0.25, 1));
	
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

		glm::vec4 colour_heat = r->get_heat_colour() * glm::vec4(glm::vec3(1), 1);
		glm::vec4 colour_spec = r->get_colour();

		if(colour_heat[3] == 0)
		{
			continue;
		}

		glm::mat4 mat = glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale;

		rmesh.add(add_dot(mat, colour_heat));

		if(sys.reactor.cursor == i)
		{
			rmesh.add(add_dot(mat * mat_cursor, {1, 0, 0, 1}));
		}

		if(r->selected)
		{
			rmesh.add(add_dot(mat * mat_select, {1, 1, 0, 1}));
		}

		if(colour_spec[3] != 0)
		{
			rmesh.add(add_dot(mat * mat_spec, colour_spec));
		}
	}

	mesh2.bind();
	mesh2.set(rmesh, GL_DYNAMIC_DRAW);
}

void Core::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();
}

