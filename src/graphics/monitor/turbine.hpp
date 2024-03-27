
#pragma once

#include "../data/model.hpp"
#include "../data/glmesh.hpp"
#include "../data/meshgen.hpp"
#include "../../audio/player.hpp"

namespace Sim::Graphics::Monitor
{

class Turbine : public Data::MeshGen
{
	glm::mat4 mat;
	Audio::Player<1, 2> a_click;

	Data::Mesh g_dial_phase;
	Data::Mesh g_dial_voltage;
	Data::Mesh g_dial_power;
	Data::Mesh g_dial_frequency;
	Data::Mesh g_switch_breaker;
	Data::Mesh m_switch_breaker;

public:

	Turbine(const Data::Model& model);
	void update(double dt) override;
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
	void remesh_slow(Data::Mesh& rmesh) override;
};

};

