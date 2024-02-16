
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Core
{
	Sim::Graphics::GLMesh mesh1, mesh2;

	Sim::Graphics::Mesh m_monitor;
	Sim::Graphics::Mesh m_buttons[9];
	Sim::Graphics::Mesh m_joystick;
	Sim::Graphics::Mesh m_scram;
	
public:

	Core();
	void init();
	void update(double dt);
	void render();
};

};

