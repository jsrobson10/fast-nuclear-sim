
#pragma once

#include "../coolant/fluid_holder.hpp"

namespace sim::electric
{

class turbine : public sim::coolant::fluid_holder
{
	const double height;
	const double diameter;
	
public:

	turbine(coolant::fluid_t type, double height, double diameter, double level);
	
	void update(double dt);
};

};

