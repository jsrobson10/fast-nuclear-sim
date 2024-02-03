
#include "turbine.hpp"

#include <cmath>
#include <iostream>

using namespace sim::electric;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h;
}

turbine::turbine(coolant::fluid_t type, double height, double diameter, double level) :
		height(height), diameter(diameter), sim::coolant::fluid_holder(type, calc_cylinder(height, diameter))
{
	this->level = level;
}

double turbine::get_bubble_hl()
{
	return (level / volume) * diameter * 0.5 / fluid.bubble_speed;
}

void turbine::update(double secs)
{
	((sim::coolant::fluid_holder*)this)->update(secs);
}


