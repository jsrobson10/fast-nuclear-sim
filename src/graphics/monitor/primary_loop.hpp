
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class primary_loop
{
	sim::graphics::glmesh mesh1, mesh2;

	sim::graphics::glmesh gm_switch_primary;
	sim::graphics::glmesh gm_switch_secondary;
	
	sim::graphics::mesh m_joystick_turbine_bypass;
	sim::graphics::mesh m_joystick_turbine_inlet;
	sim::graphics::mesh m_switch_primary;
	sim::graphics::mesh m_switch_secondary;

	void toggle_primary_pump();
	
public:

	primary_loop();
	void init();
	void update();
	void render();
};

};

