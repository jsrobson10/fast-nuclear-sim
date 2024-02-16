
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Turbine
{
	Sim::Graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	Sim::Graphics::GLMesh gm_synchroscope_dial;
	Sim::Graphics::GLMesh gm_switch_breaker;
	Sim::Graphics::Mesh m_switch_breaker;

public:

	Turbine();
	void init();
	void update(double dt);
	void render();
};

};

