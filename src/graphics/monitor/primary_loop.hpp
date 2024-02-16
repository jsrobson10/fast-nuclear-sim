
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class primary_loop
{
	glmesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	glmesh gm_switch_pump;
	glmesh gm_switch_bypass;
	glmesh gm_switch_inlet;
	
	mesh m_joystick_turbine_bypass;
	mesh m_joystick_turbine_inlet;

	mesh m_switch_pump;
	mesh m_switch_bypass;
	mesh m_switch_inlet;
	
public:

	primary_loop();
	void init();
	void update(double dt);
	void render();
};

};

