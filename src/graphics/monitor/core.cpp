
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
	mesh1.model_matrix = mesh2.model_matrix = locations::monitors[2];
	mesh1.colour_matrix = {
		1, 1, 1, 1,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	std::stringstream ss;

	ss << "Reactor Core";

	mesh1.bind();
	mesh1.load_text(ss.str().c_str(), 0.05);
	mesh2.bind();
	mesh2.load_model("../assets/model/", "reactor_core_interface.stl");
}

void core::update(sim::system& sys)
{
}

void core::render()
{
	mesh1.bind(); mesh1.render();
	mesh2.bind(); mesh2.render();
}

