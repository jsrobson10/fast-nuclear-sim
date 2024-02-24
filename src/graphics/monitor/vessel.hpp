
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Monitor
{

class Vessel
{
	glm::mat4 mat;
	
public:

	Vessel();
	void init(Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void render();
};

};

