
#include "turbine.hpp"
#include "../system.hpp"

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

void turbine::update(double secs)
{
	sim::system& sys = sim::system::active;
	auto& o = *sys.turbine.get();

	((sim::coolant::fluid_holder*)this)->update(secs);

/*	if(level + o.level > o.volume)
	{
		level += o.level - o.volume;
		o.level = o.volume;
	}

	else
	{
		o.level += level;
		level = 0;
	}

	balance_steam(o);*/
}


