
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Core
{
	Sim::Graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;

	Sim::Graphics::Mesh m_monitor;
	Sim::Graphics::Mesh m_buttons[9];
	Sim::Graphics::Mesh m_joystick;
	Sim::Graphics::Mesh m_scram;
	
public:

	bool is_dirty = false;

	Core();
	void init();
	void update(double dt);
	void render();
};

};

