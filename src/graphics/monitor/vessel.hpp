
#pragma once

#include "../mesh/mesh.hpp"
#include "../../system.hpp"

namespace sim::graphics::monitor
{

class vessel
{
	sim::graphics::mesh mesh1, mesh2;
	
public:

	vessel();
	void init();
	void update(sim::system& sys);
	void render();
};

};

