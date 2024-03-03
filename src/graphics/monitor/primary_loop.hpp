
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class PrimaryLoop : public MeshGen
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

	PrimaryLoop(const Model& model);
	virtual void update(double dt);
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms);
	virtual void remesh_static(Mesh& rmesh);
	virtual void remesh_slow(Mesh& rmesh);
};

};

