
#pragma once

#include "../data/model.hpp"
#include "../data/meshgen.hpp"
#include "../../audio/player.hpp"

namespace Sim::Graphics::Monitor
{

class SecondaryLoop : public Data::MeshGen
{
	glm::mat4 mat;
	Audio::Player<2, 2> a_click;

	Data::Mesh g_switch_2;
	Data::Mesh g_switch_3;

	Data::Mesh m_joystick_turbine_bypass;
	Data::Mesh m_joystick_turbine_inlet;
	Data::Mesh m_switch_2;
	Data::Mesh m_switch_3;
	
public:

	SecondaryLoop(const Data::Model& model);
	void update(double dt) override;
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void remesh_slow(Data::Mesh& rmesh) override;
};

};

