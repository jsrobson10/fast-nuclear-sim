
#pragma once

#include "../mesh/glmesh.hpp"

namespace Sim::Graphics::Widget
{

struct Clock
{
	GLMesh data;
	
	void update(double dt);
	void render();
};

};

