
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core.hpp"
#include "../locations.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <sstream>

using namespace sim::graphics::monitor;

core::core()
{

}

void core::init()
{
	mesh1.model_matrix = locations::monitors[2];
	mesh1.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	sim::graphics::mesh rmesh1, rmesh2;
	
	rmesh1.load_text("Reactor Core", 0.04);
	rmesh2.load_model("../assets/model/", "reactor_core_interface_circle.stl");
	rmesh1.add(rmesh2, glm::mat4(1));

	mesh1.bind();
	mesh1.set(rmesh1, GL_STATIC_DRAW);
	rmesh2.load_model("../assets/model/", "reactor_core_interface_cell.stl");
	mesh2.bind();
	mesh2.set(rmesh2, GL_STATIC_DRAW);
}

void core::render(sim::system& sys)
{
	double step = 1 / (sys.vessel->diameter / sys.reactor->cell_width * 0.8);
	double sx = 0.5 - (sys.reactor->width - 1) * step / 2.0;
	double sy = 0.5 - (sys.reactor->height - 1) * step / 2.0;

	glm::mat4 mat_scale = glm::scale(glm::mat4(1), glm::vec3(step * 0.5, step * 0.5, 1));

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

		if(!r->should_display() || colour[3] == 0)
		{
			continue;
		}

		mesh2.model_matrix = mesh1.model_matrix * glm::translate(glm::mat4(1), glm::vec3(ox, oy, 0)) * mat_scale;
		mesh2.colour_matrix = glm::mat4({
			colour[0], colour[1], colour[2], colour[3],
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		});

		mesh2.uniform();
		mesh2.render();
	}
}

