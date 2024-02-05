
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class condenser : public fluid_holder
{
	const double height;
	const double diameter;

public:

	condenser(fluid_t type, double height, double diameter, double mass, double level);
	
	void update(double dt);
};

};

