
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class vessel
{
	sim::graphics::glmesh mesh1, mesh2;
	
public:

	vessel();
	void init();
	void update();
	void render();
};

};

