
#pragma once

#include "../mesh/model.hpp"

namespace Sim::Graphics::Monitor
{

class Vessel
{
	glm::mat4 mat;
	
public:

	Vessel();
	void init(const Model& model, Mesh& rmesh);
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void remesh_fast(Mesh& rmesh);
	void render();
};

};

