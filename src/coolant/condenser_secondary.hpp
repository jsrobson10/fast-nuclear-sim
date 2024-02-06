
#pragma once

#include "fluid_holder.hpp"
#include "condenser.hpp"

namespace sim::coolant
{

class condenser_secondary : public fluid_holder
{
	condenser* const primary;
	
public:

	condenser_secondary(condenser* primary, double volume, double level);

	void update(double dt);
};

};

