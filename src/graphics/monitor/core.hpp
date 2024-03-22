
#pragma once

#include "../data/model.hpp"
#include "../data/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Core : public Data::MeshGen
{
	glm::mat4 mat;

	Data::Mesh m_monitor;
	Data::Mesh m_buttons[9];
	Data::Mesh m_joystick;
	Data::Mesh m_scram;

public:

	Core(const Data::Model& model);
	void update(double dt) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void remesh_slow(Data::Mesh& rmesh) override;
};

};

