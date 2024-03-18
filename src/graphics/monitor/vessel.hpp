
#pragma once

#include "../data/model.hpp"
#include "../data/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Vessel : public Data::MeshGen
{
	glm::mat4 mat;
	
public:

	Vessel(const Data::Model& model);
	void remesh_static(Data::Mesh& rmesh) override;
	void remesh_slow(Data::Mesh& rmesh) override;
};

};

