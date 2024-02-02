
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class condenser : public fluid_holder
{
	const double height;
	const double diameter;
	
	virtual double get_bubble_hl();

public:

	condenser(fluid_t type, double height, double diameter, double level);
	
	void update(double dt);
};

};

