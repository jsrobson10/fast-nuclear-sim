
#pragma once

#include "../data/meshgen.hpp"

namespace Sim::Graphics::Equipment
{

class Pool : public Data::MeshGen
{
	Data::Mesh g_pool;

public:

	Pool(const Data::Model& model);
	void get_static_transforms(std::vector<glm::mat4>& transforms) override;
	void remesh_static(Data::Mesh& rmesh) override;
};

};

