
#include "vessel.hpp"
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
	mesh2.model_matrix = glm::translate(mesh1.model_matrix, glm::vec3(3.0 / 8.0, 0, 0));

	mesh1.colour_matrix = mesh2.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;

	ss << "Reactor Vessel\n\n";
	ss << "Heat\n";
	ss << "Steam\n";
	ss << "Pressure\n";
	ss << "Level\n";
	ss << "Void Ratio\n";

	mesh1.bind();
	mesh1.load_text(ss.str().c_str(), 0.05);
}

void vessel::update(sim::system& sys)
{
	std::stringstream ss;

	ss << "\n\n";
	ss << sys.vessel.get_heat() << " C\n";
	ss << sys.vessel.get_steam() << " g\n";
	ss << (sys.vessel.get_pressure() * 0.001) << " kPa\n";
	ss << sys.vessel.get_level() << " / " << sys.vessel.get_volume() << " L\n";
	ss << (sys.vessel.get_void_ratio() * 100) << " %\n";

	mesh2.bind();
	mesh2.load_text(ss.str().c_str(), 0.05);
}

void vessel::render()
{
	mesh1.bind(); mesh1.render();
	mesh2.bind(); mesh2.render();
}

