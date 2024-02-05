
#pragma once

#include "../mesh/glmesh.hpp"

namespace sim::graphics::widget
{

struct clock
{
	double at = 3600 * 12;
	glmesh data;
	
	void update(double dt);
	void render();
};

};

