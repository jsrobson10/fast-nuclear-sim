
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class primary_loop
{
	sim::graphics::glmesh mesh1, mesh2;
	
	sim::graphics::mesh m_joystick_turbine_bypass;
	sim::graphics::mesh m_joystick_turbine_inlet;
	
public:

	primary_loop();
	void init();
	void update();
	void render();
};

};

