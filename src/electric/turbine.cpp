
#include "turbine.hpp"
#include "../system.hpp"
#include "../util/math.hpp"

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

turbine::turbine(const Json::Value& node, coolant::condenser* condenser) :
		condenser(condenser),
		length(node["length"].asDouble()),
		diameter(node["diameter"].asDouble()),
		friction(node["friction"].asDouble()),
		sim::coolant::fluid_holder(node)
{
	velocity = node["velocity"].asDouble();
}

void turbine::update(double dt)
{
	double work = get_rpm() / 60 * dt * friction;
	velocity = std::max(velocity - work, 0.0);
}

double turbine::get_rpm() const
{
	return velocity / (M_PI * extra_mass * 0.001 * diameter * diameter * 0.25);
}

void turbine::add_gas(double steam, double air, double t)
{
	double joules = (steam + air) * fluid.jPg;
	velocity = std::max(velocity + util::calc_work(joules, extra_mass), 0.0);
	condenser->add_gas(steam, air, t);
}

turbine::operator Json::Value() const
{
	Json::Value node(fluid_holder::operator::Json::Value());

	node["length"] = length;
	node["diameter"] = diameter;
	node["velocity"] = velocity;
	node["friction"] = friction;
	
	return node;
}

