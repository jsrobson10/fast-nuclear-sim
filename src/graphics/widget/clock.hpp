
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Widget
{

struct Clock
{
	double dt;
	
	void update(double dt);
	void remesh_slow(Mesh& rmesh);
	void render();
};

};

