
#include "evaporator.hpp"

#include <cmath>

using namespace sim::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h;
}

evaporator::evaporator(fluid_t type, double height, double diameter, double mass, double level) :
		height(height), diameter(diameter), fluid_holder(type, calc_cylinder(height, diameter), mass)
{
	this->level = level;
}

