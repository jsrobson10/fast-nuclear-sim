
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::monitor
{

class PrimaryLoop
{
	GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	GLMesh gm_switch_pump;
	GLMesh gm_switch_bypass;
	GLMesh gm_switch_inlet;
	
	Mesh m_joystick_turbine_bypass;
	Mesh m_joystick_turbine_inlet;

	Mesh m_switch_pump;
	Mesh m_switch_bypass;
	Mesh m_switch_inlet;
	
public:

	PrimaryLoop();
	void init();
	void update(double dt);
	void render();
};

};

