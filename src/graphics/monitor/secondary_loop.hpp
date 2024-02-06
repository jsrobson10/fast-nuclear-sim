
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class secondary_loop
{
	sim::graphics::glmesh mesh1, mesh2;

	sim::graphics::glmesh gm_switch_2;
	sim::graphics::glmesh gm_switch_3;
	
	sim::graphics::mesh m_joystick_turbine_bypass;
	sim::graphics::mesh m_joystick_turbine_inlet;
	sim::graphics::mesh m_switch_2;
	sim::graphics::mesh m_switch_3;

	void toggle_secondary_pump();
	void toggle_freight_pump();
	
public:

	secondary_loop();
	void init();
	void update(double dt);
	void render();
};

};

