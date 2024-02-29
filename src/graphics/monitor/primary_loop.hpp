
#pragma once

#include "../mesh/model.hpp"

namespace Sim::Graphics::Monitor
{

class PrimaryLoop
{
	glm::mat4 mat;

	Mesh m_joystick_turbine_bypass;
	Mesh m_joystick_turbine_inlet;

	Mesh g_switch_pump;
	Mesh g_switch_bypass;
	Mesh g_switch_inlet;
	
	Mesh m_switch_pump;
	Mesh m_switch_bypass;
	Mesh m_switch_inlet;
	
public:

	PrimaryLoop();
	void init(const Model& model, Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void render();
};

};

