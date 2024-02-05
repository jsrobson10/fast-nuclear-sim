
#include "vessel.hpp"
#include "../../util/constants.hpp"
#include "../../conversions/temperature.hpp"
#include "../fuel/half_life.hpp"

#include <iostream>
#include <cmath>

using namespace sim::reactor::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h;
}

vessel::vessel(sim::coolant::fluid_t fluid, double height, double diameter, double mass, double level) :
		sim::coolant::fluid_holder(fluid, calc_cylinder(height, diameter), mass),
		height(height), diameter(diameter)
{
	this->level = level;
}

double vessel::get_steam_suspended() const
{
	return steam_suspended;
}

double vessel::get_void_ratio() const
{
	double density = get_steam_density();

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

double vessel::get_bubble_hl() const
{
	return (level / volume) * height * 0.5 / fluid.bubble_speed;
}

void vessel::update(double secs)
{
	double s = steam;

	((sim::coolant::fluid_holder*)this)->update(secs);

	double diff = steam - s;
	double hl = get_bubble_hl();

	if(diff > 0)
	{
		steam_suspended += diff;
	}

	if(hl > 0)
	{
		steam_suspended *= reactor::fuel::half_life::get(secs, get_bubble_hl());
	}

	else
	{
		steam_suspended = 0;
	}
}

