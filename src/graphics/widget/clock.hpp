
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::widget
{

struct clock
{
	glmesh data;
	
	void update(double dt);
	void render();
};

};

