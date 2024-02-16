
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::monitor
{

class SecondaryLoop
{
	Sim::Graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	Sim::Graphics::GLMesh gm_switch_2;
	Sim::Graphics::GLMesh gm_switch_3;
	
	Sim::Graphics::Mesh m_joystick_turbine_bypass;
	Sim::Graphics::Mesh m_joystick_turbine_inlet;
	Sim::Graphics::Mesh m_switch_2;
	Sim::Graphics::Mesh m_switch_3;
	
public:

	SecondaryLoop();
	void init();
	void update(double dt);
	void render();
};

};

