
#include "vessel.hpp"
#include "../../constants.hpp"
#include "../../conversions/temperature.hpp"
#include "../fuel/half_life.hpp"

#include <cmath>

using namespace sim::reactor::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h;
}

vessel::vessel(sim::coolant::fluid_t fluid, double height, double diameter, double level) :
		sim::coolant::fluid_holder(fluid, calc_cylinder(height, diameter)),
		height(height), diameter(diameter)
{
	this->level = level;
}

double vessel::get_bubble_hl()
{
	return (level / volume) * height * 0.5 / fluid.bubble_speed;
}

void vessel::update(double secs)
{
	((sim::coolant::fluid_holder*)this)->update(secs);
}

