
#include "condenser.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

Condenser::Condenser(Fluid type, double height, double diameter, double mass, double level) :
		height(height), diameter(diameter), FluidHolder(type, calc_cylinder(height, diameter), mass)
{
	this->level = level;
}

Condenser::Condenser(const Json::Value& node) :
		height(node["height"].asDouble()),
		diameter(node["diameter"].asDouble()),
		FluidHolder(node)
{

}

void Condenser::update(double secs)
{
	update_base(secs);
}

Condenser::operator Json::Value() const
{
	Json::Value node(FluidHolder::operator::Json::Value());

	node["height"] = height;
	node["diameter"] = diameter;
	
	return node;
}

