
#pragma once

#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Reactor : public MeshGen
{
	Mesh g_control_rod_lift, g_control_rod_base;

public:

	Reactor(const Model& model);
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms);
	virtual void remesh_static(Mesh& rmesh);
};

};

