
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::widget
{

struct Clock
{
	GLMesh data;
	
	void update(double dt);
	void render();
};

};

