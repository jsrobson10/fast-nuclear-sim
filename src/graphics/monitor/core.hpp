
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class core
{
	sim::graphics::glmesh mesh1, mesh2;

	sim::graphics::mesh m_monitor;
	sim::graphics::mesh m_buttons[9];
	sim::graphics::mesh m_joystick;
	sim::graphics::mesh m_scram;
	
public:

	core();
	void init();
	void update();
	void render();
};

};

