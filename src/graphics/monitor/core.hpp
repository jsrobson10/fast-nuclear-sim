
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Core : public MeshGen
{
	glm::mat4 mat;

	Mesh m_monitor;
	Mesh m_buttons[9];
	Mesh m_joystick;
	Mesh m_scram;
	
public:

	Core(const Model& model);
	virtual void update(double dt);
	virtual void remesh_static(Mesh& rmesh);
	virtual void remesh_slow(Mesh& rmesh);
};

};

