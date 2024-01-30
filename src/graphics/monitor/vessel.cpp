
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vessel.hpp"
#include "../../reactor/rod.hpp"
#include "../../reactor/control/boron_rod.hpp"
#include "../locations.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <sstream>

using namespace sim::graphics::monitor;

vessel::vessel()
{

}

void vessel::init()
{
	mesh1.model_matrix = locations::monitors[1];
	mesh2.model_matrix = glm::translate(mesh1.model_matrix, glm::vec3(0.5, 0, 0));

	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;
	sim::graphics::mesh rmesh;

	ss << "Reactor Vessel\n\n";
	ss << "Heat\n";
	ss << "Steam\n";
	ss << "Pressure\n";
	ss << "Level\n";
	ss << "Void Ratio\n\n\n\n";
	ss << "Reactor Core\n\n";
	ss << "Temperature\nMin\nMax\n\n";
	ss << "Neutron Flux\nSlow\nFast\n\n";
	ss << "Control Rods\nMin\nMax\nSpeed\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh1.bind();
	mesh1.set(rmesh, GL_STATIC_DRAW);
}

static double show(double v, double m)
{
	return std::round(v * m) / m;
}

static double show(double v)
{
	return std::round(v * 1e3) * 1e-3;
}

void vessel::update()
{
	sim::system& sys = sim::system::active;

	std::stringstream ss;
	sim::graphics::mesh rmesh;

	double temp_min, temp_max;
	double crod_min = INFINITY, crod_max = -INFINITY;

	sys.reactor->get_stats(sim::reactor::rod::val_t::HEAT, temp_min, temp_max);

	for(int i = 0; i < sys.reactor->size; i++)
	{
		sim::reactor::rod* r = sys.reactor->rods[i].get();

		if(r->get_id() != 5)
		{
			continue;
		}

		auto br = (sim::reactor::control::boron_rod*)r;
		double v = br->get_inserted();

		if(v > crod_max)
		{
			crod_max = v;
		}

		if(v < crod_min)
		{
			crod_min = v;
		}
	}

	ss << "\n\n";
	ss << show( sys.vessel->get_heat() ) << " C\n";
	ss << show( sys.vessel->get_steam() ) << " g\n";
	ss << show( sys.vessel->get_pressure() * 0.001 ) << " kPa\n";
	ss << show( sys.vessel->get_level() ) << " / " << show( sys.vessel->get_volume() ) << " L\n";
	ss << show( sys.vessel->get_void_ratio() * 100 ) << " %\n\n\n\n\n\n\n";
	ss << show( temp_min ) << " C\n";
	ss << show( temp_max ) << " C\n\n\n";
	ss << sys.reactor->get_total(sim::reactor::rod::val_t::N_SLOW) << " mol\n";
	ss << sys.reactor->get_total(sim::reactor::rod::val_t::N_FAST) << " mol\n\n\n";
	ss << show( crod_min * 100 ) << " %\n";
	ss << show( crod_max * 100 ) << " %\n";
	ss << show( sys.reactor->rod_speed * 100, 1e6 ) << " %/s";
	if(sys.reactor->rod_speed == 0) ss << " (Stopped)";
	ss << "\n";

	rmesh.load_text(ss.str().c_str(), 0.04);
	mesh2.bind();
	mesh2.set(rmesh, GL_DYNAMIC_DRAW);
}

void vessel::render()
{
	mesh1.bind();
	mesh1.uniform();
	mesh1.render();

	mesh2.bind();
	mesh2.uniform();
	mesh2.render();
}

