
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::monitor
{

class Core
{
	sim::graphics::GLMesh mesh1, mesh2;

	sim::graphics::Mesh m_monitor;
	sim::graphics::Mesh m_buttons[9];
	sim::graphics::Mesh m_joystick;
	sim::graphics::Mesh m_scram;
	
public:

	Core();
	void init();
	void update(double dt);
	void render();
};

};

