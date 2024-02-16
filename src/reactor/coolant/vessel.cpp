
#include "vessel.hpp"
#include "../../util/constants.hpp"
#include "../../conversions/temperature.hpp"
#include "../fuel/half_life.hpp"

#include <iostream>
#include <cmath>

using namespace Sim::Reactor::Coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

Vessel::Vessel(Sim::Coolant::Fluid fluid, double height, double diameter, double mass, double level, double bubble_hl) :
		Sim::Coolant::FluidHolder(fluid, calc_cylinder(height, diameter), mass),
		height(height), diameter(diameter), bubble_hl(bubble_hl)
{
	this->level = level;
}

Vessel::Vessel(const Json::Value& node) :
		Sim::Coolant::FluidHolder(node),
		height(node["height"].asDouble()),
		diameter(node["diameter"].asDouble()),
		bubble_hl(node["bubble_hl"].asDouble())
{
	steam_suspended = node["steam_suspended"].asDouble();
}

double Vessel::get_steam_suspended() const
{
	return steam_suspended;
}

double Vessel::get_void_ratio() const
{
	double density = get_gas_density();

	if(density == 0)
	{
		return 0;
	}

	double s = steam_suspended / density;
	double m = level + s;
	
	if(m == 0)
	{
		return 0;
	}

	return s / m;
}

void Vessel::update(double secs)
{
	double steam_last = steam;

	update_base(secs);
	
	double diff = steam - steam_last;

	steam_last = steam;
	steam_suspended += diff;
	steam_suspended *= Reactor::Fuel::half_life::get(secs, bubble_hl);

	if(steam_suspended < 0)
	{
		steam_suspended = 0;
	}
}

Vessel::operator Json::Value() const
{
	Json::Value node(FluidHolder::operator::Json::Value());

	node["height"] = height;
	node["diameter"] = diameter;
	node["bubble_hl"] = bubble_hl;
	node["steam_suspended"] = steam_suspended;
	
	return node;
}

