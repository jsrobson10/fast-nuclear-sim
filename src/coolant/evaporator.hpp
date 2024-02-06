
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class evaporator : public fluid_holder
{
	const double height;
	const double diameter;
	
public:

	evaporator(fluid_t type, double height, double diameter, double mass, double level);
};

};

