
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class turbine
{
	sim::graphics::glmesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	sim::graphics::glmesh gm_synchroscope_dial;
	sim::graphics::glmesh gm_switch_breaker;
	sim::graphics::mesh m_switch_breaker;

public:

	turbine();
	void init();
	void update(double dt);
	void render();
};

};

