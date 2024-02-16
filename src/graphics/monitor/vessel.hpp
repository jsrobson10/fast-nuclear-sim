
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::monitor
{

class Vessel
{
	Sim::Graphics::GLMesh mesh1, mesh2;
	double clock_at = 0, clock_now = 0;
	
public:

	Vessel();
	void init();
	void update(double dt);
	void render();
};

};

