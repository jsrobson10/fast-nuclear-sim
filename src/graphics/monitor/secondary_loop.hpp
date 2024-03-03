
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class SecondaryLoop : public MeshGen
{
	glm::mat4 mat;

	Mesh g_switch_2;
	Mesh g_switch_3;
	
	Mesh m_joystick_turbine_bypass;
	Mesh m_joystick_turbine_inlet;
	Mesh m_switch_2;
	Mesh m_switch_3;
	
public:

	SecondaryLoop(const Model& model);
	virtual void update(double dt);
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms);
	virtual void remesh_static(Mesh& rmesh);
	virtual void remesh_slow(Mesh& rmesh);
};

};

