
#pragma once

#include "../mesh/model.hpp"
#include "../mesh/meshgen.hpp"

namespace Sim::Graphics::Monitor
{

class Vessel : public MeshGen
{
	glm::mat4 mat;
	
public:

	Vessel(const Model& model, Mesh& rmesh);
	virtual void update(double dt);
	virtual void remesh_slow(Mesh& rmesh);
	virtual void remesh_fast(Mesh& rmesh);
	virtual void render();
};

};

