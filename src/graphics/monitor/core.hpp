
#pragma once

#include "../mesh/glmesh.hpp"
#include "../../system.hpp"

namespace sim::graphics::monitor
{

class core
{
	sim::graphics::glmesh mesh1, mesh2;
	
public:

	core();
	void init();
	void render(sim::system& sys);
};

};

