
#include "condenser.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

condenser::condenser(fluid_t type, double height, double diameter, double mass, double level) :
		height(height), diameter(diameter), fluid_holder(type, calc_cylinder(height, diameter), mass)
{
	this->level = level;
}

void condenser::update(double secs)
{
	update_base(secs);
}

condenser::operator Json::Value() const
{
	Json::Value node(fluid_holder::operator::Json::Value());

	node["height"] = height;
	node["diameter"] = diameter;
	
	return node;
}

