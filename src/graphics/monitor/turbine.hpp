
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class Turbine
{
	sim::graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	sim::graphics::GLMesh gm_synchroscope_dial;
	sim::graphics::GLMesh gm_switch_breaker;
	sim::graphics::Mesh m_switch_breaker;

public:

	Turbine();
	void init();
	void update(double dt);
	void render();
};

};

