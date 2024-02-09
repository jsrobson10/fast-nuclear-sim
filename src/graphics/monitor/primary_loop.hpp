
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class primary_loop
{
	sim::graphics::glmesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	sim::graphics::glmesh gm_switch_1;
	
	sim::graphics::mesh m_joystick_turbine_bypass;
	sim::graphics::mesh m_joystick_turbine_inlet;
	sim::graphics::mesh m_switch_1;

	void toggle_primary_pump();
	
public:

	primary_loop();
	void init();
	void update(double dt);
	void render();
};

};

