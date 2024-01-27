
#pragma once

#include "../mesh/mesh.hpp"
#include "../../system.hpp"

namespace sim::graphics::monitor
{

class core
{
	sim::graphics::mesh mesh1, mesh2;
	
public:

	core();
	void init();
	void update(sim::system& sys);
	void render();
};

};

