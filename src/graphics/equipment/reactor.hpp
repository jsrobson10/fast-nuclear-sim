
#pragma once

#include "../data/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Reactor : public Data::MeshGen
{
	Data::Mesh g_control_rod_lift, g_control_rod_base;

public:

	Reactor(const Data::Model& model);
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
};

};

