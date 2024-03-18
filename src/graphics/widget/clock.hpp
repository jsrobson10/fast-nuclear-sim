
#pragma once

#include "../data/glmesh.hpp"

namespace Sim::Graphics::Widget
{

struct Clock
{
	double dt;
	
	void update(double dt);
	void remesh_slow(Data::Mesh& rmesh);
};

};

