
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Core
{
	glm::mat4 mat;
	Mesh m_monitor;
	Mesh m_buttons[9];
	Mesh m_joystick;
	Mesh m_scram;
	
public:

	Core();
	void init(Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void remesh(Mesh& rmesh, bool fast);
	void render();
};

};

