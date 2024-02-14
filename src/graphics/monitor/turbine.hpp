
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class turbine
{
	sim::graphics::glmesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

public:

	turbine();
	void init();
	void update(double dt);
	void render();
};

};

