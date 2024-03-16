
#pragma once

#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Generator : public MeshGen
{
	Mesh g_rotor;

public:

	Generator(const Model& model);
	virtual void get_static_transforms(std::vector<glm::mat4>& transforms);
	virtual void remesh_static(Mesh& rmesh);
};

};

