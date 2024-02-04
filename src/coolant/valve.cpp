
#include "valve.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

valve::valve(fluid_holder* src, fluid_holder* dst, double max) : src(src), dst(dst), max(max)
{

}

void valve::add_open_speed(double v)
{
	speed += v;
}

void valve::clear_open_speed()
{
	speed = 0;
}

void valve::update(double dt)
{
	state += speed * dt;

	if(state > 1) state = 1;
	if(state < 0) state = 0;

	double r = max * state; // L
	double m = r * dt;//1 - std::pow(0.5, dt * -std::log2(1 - r));
	
	double pressure1 = src->get_pressure();
	double pressure2 = dst->get_pressure();
	double density1 = src->get_steam_density(); // g/L
	double density2 = dst->get_steam_density(); // g/L
	double diff = (pressure1 - pressure2) * m;
	double temp, mass;

	if(diff > 0)
	{
		temp = src->get_heat();
		mass = diff * src->get_steam_density();
	}

	else
	{
		temp = dst->get_heat();
		mass = diff * dst->get_steam_density();
	}

	src->add_steam(-mass, temp);
	dst->add_steam(mass, temp);

	this->flow = mass / dt;
}

