
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class SecondaryLoop
{
	glm::mat4 mat;

	Mesh g_switch_2;
	Mesh g_switch_3;
	
	Mesh m_joystick_turbine_bypass;
	Mesh m_joystick_turbine_inlet;
	Mesh m_switch_2;
	Mesh m_switch_3;
	
public:

	SecondaryLoop();
	void init(Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void render();
};

};

