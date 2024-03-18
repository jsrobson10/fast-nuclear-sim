
#pragma once

#include "../data/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Generator : public Data::MeshGen
{
	Data::Mesh g_rotor;

public:

	Generator(const Data::Model& model);
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
};

};

