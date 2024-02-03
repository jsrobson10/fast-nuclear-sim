
#include "condenser.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h;
}

condenser::condenser(fluid_t type, double height, double diameter, double level) :
		height(height), diameter(diameter), fluid_holder(type, calc_cylinder(height, diameter))
{
	this->level = level;
}

double condenser::get_bubble_hl()
{
	return (level / volume) * diameter * 0.5 / fluid.bubble_speed;
}

void condenser::update(double secs)
{
	((sim::coolant::fluid_holder*)this)->update(secs);
}


