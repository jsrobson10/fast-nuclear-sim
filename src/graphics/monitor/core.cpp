
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "../locations.hpp"
#include "../input/focus.hpp"
#include "../mesh/arrays.hpp"

#include <glm/ext/matrix_transform.hpp>

#include <iostream>
#include <sstream>

using namespace sim::graphics;
using namespace sim::graphics::monitor;

struct core_focus_t : public focus::focus_t
{
	sim::system* sys;
	
	core_focus_t(sim::system& sys)
	{
		this->sys = &sys;
	}

	void on_keypress(int key, int sc, int action, int mods)
	{
		if(action != GLFW_PRESS)
		{
			return;
		}

	}

	void on_mouse_button(int button, int action, int mods)
	{

	}

	void on_cursor_pos(double x, double y)
	{

	}

	void set_all(bool state)
	{
		for(int i = 0; i < sys->reactor->rods.size(); i++)
		{
			sim::reactor::rod* r = sys->reactor->rods[i].get();

			if(r->should_select() && (r->is_selected() != state))
			{
				r->toggle_selected();
			}
		}
	}

	void toggle_auto()
	{
		//TODO
	}

	void on_charcode(unsigned int c)
	{
		switch(c)
		{
		case 'a': case 'A':
			sys->reactor->move_cursor(-1);
			break;
		case 'd': case 'D':
			sys->reactor->move_cursor(1);
			break;
		case 's': case 'S':
			sys->reactor->toggle_selected();
			break;
		case 'w': case 'W':
			toggle_auto();
			break;
		case 'q': case 'Q':
			set_all(false);
			break;
		case 'e': case 'E':
			set_all(true);
			break;
		}
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
	
	mesh_click.load_model("../assets/model/", "reactor_core_input.stl");
}

void core::update(sim::system& sys)
{
	if(mesh_click.check_focus(2))
	{
		focus::set(std::make_unique<core_focus_t>(sys));
	}
}

void core::render(sim::system& sys)
{
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

		if(sys.reactor->cursor == i && r->should_select())
		{
			mesh2.model_matrix = mat * mat_cursor;
			mesh2.colour_matrix = arrays::colour({1, 0, 0, 1});
			mesh2.uniform();
			mesh2.render();
		}

		if(r->is_selected())
		{
			mesh2.model_matrix = mat * mat_select;
			mesh2.colour_matrix = arrays::colour({1, 1, 0, 1});
			mesh2.uniform();
			mesh2.render();
		}
	}
}

