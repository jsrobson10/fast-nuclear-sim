
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "helpers.hpp"
#include "turbine.hpp"
#include "../locations.hpp"
#include "../../system.hpp"
#include "../../coolant/valve.hpp"
#include "../input/focus.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <iostream>

using namespace sim::graphics;
using namespace sim::graphics::monitor;

turbine::turbine()
{

}

void turbine::init()
{
	mesh1.model_matrix = locations::monitors[4];
	mesh2.model_matrix = glm::translate(mesh1.model_matrix, glm::vec3(0.5, 0, 0));

	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;
	sim::graphics::mesh rmesh;

	ss << "Turbine\n\n";
	ss << "Heat\nPressure\nSpeed\n\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);
}

void turbine::update(double dt)
{
	system& sys = sim::system::active;
	clock_now += dt;

	if(clock_at + 1.0/30.0 < clock_now)
	{
		std::stringstream ss;
		sim::graphics::mesh rmesh;
		clock_at += 1.0/30.0;

		ss << "\n\n";
		ss << show( sys.turbine->get_heat() ) << " C\n";
		ss << show( sys.turbine->get_pressure() / 1000 ) << " kPa\n";
		ss << show( sys.turbine->get_rpm() ) << " r/min\n";
		
		rmesh.load_text(ss.str().c_str(), 0.04);
		mesh2.bind();
		mesh2.set(rmesh, GL_DYNAMIC_DRAW);
	}
}

void turbine::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();
}

