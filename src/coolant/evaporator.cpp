
#include "evaporator.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

evaporator::evaporator(fluid_t type, double height, double diameter, double mass, double level) :
		height(height),
		diameter(diameter),
		fluid_holder(type, calc_cylinder(height, diameter), mass)
{
	this->level = level;
}

double evaporator::get_steam_output()
{
	return steam_output;
}

void evaporator::update(double dt)
{
	((fluid_holder*)this)->update(dt);
/*
	double m = std::pow(0.5, dt / 3600);
	double steam_out = steam * (1 - m);
	steam *= m;

	steam_output = steam_out / dt;*/
}

