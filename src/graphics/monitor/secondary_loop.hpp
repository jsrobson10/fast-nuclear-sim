
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class SecondaryLoop
{
	sim::graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	sim::graphics::GLMesh gm_switch_2;
	sim::graphics::GLMesh gm_switch_3;
	
	sim::graphics::Mesh m_joystick_turbine_bypass;
	sim::graphics::Mesh m_joystick_turbine_inlet;
	sim::graphics::Mesh m_switch_2;
	sim::graphics::Mesh m_switch_3;
	
public:

	SecondaryLoop();
	void init();
	void update(double dt);
	void render();
};

};

