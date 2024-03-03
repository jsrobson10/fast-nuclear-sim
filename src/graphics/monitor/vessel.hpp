
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Vessel : public MeshGen
{
	glm::mat4 mat;
	
public:

	Vessel(const Model& model);
	virtual void remesh_static(Mesh& rmesh);
	virtual void remesh_slow(Mesh& rmesh);
};

};

