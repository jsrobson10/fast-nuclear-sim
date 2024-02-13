
#include "turbine.hpp"
#include "../system.hpp"

#include <cmath>
#include <iostream>

using namespace sim::electric;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

turbine::turbine(coolant::fluid_t type, coolant::condenser* condenser, double length, double diameter, double mass) :
		length(length), diameter(diameter), condenser(condenser),
		sim::coolant::fluid_holder(type, calc_cylinder(length, diameter), mass)
{
	
}

void turbine::update(double dt)
{
	
}

void turbine::add_gas(double steam, double air, double t, double e)
{
	condenser->add_gas(steam, air, t, e);
}

turbine::operator Json::Value() const
{
	Json::Value node(fluid_holder::operator::Json::Value());

	node["length"] = length;
	node["diameter"] = diameter;
	node["velocity"] = velocity;
	
	return node;
}

