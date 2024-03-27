
#pragma once

#include "../data/model.hpp"
#include "../data/meshgen.hpp"
#include "../../audio/player.hpp"

namespace Sim::Graphics::Monitor
{

class PrimaryLoop : public Data::MeshGen
{
	glm::mat4 mat;
	Audio::Player<3, 2> a_click;

	Data::Mesh m_joystick_turbine_bypass;
	Data::Mesh m_joystick_turbine_inlet;

	Data::Mesh g_switch_pump;
	Data::Mesh g_switch_bypass;
	Data::Mesh g_switch_inlet;
	
	Data::Mesh m_switch_pump;
	Data::Mesh m_switch_bypass;
	Data::Mesh m_switch_inlet;
	
public:

	PrimaryLoop(const Data::Model& model);
	void update(double dt) override;
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void remesh_slow(Data::Mesh& rmesh) override;
};

};

