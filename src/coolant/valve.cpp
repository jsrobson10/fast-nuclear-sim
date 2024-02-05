
#include "valve.hpp"
#include "../conversions/temperature.hpp"
#include "../util/constants.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

valve::valve(fluid_holder* src, fluid_holder* dst, double state, double max) : src(src), dst(dst), max(max)
{
	this->state = state;
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

	if(src->get_steam_volume() == 0 || dst->get_steam_volume() == 0)
	{
		flow = 0;
		return;
	}

	double pressure1 = src->get_pressure(); // Pa
	double pressure2 = dst->get_pressure();
	
	int overshoots = 0;
	double m = max * state * dt;
	double temp, mass;

	for(;;)
	{
		double diff = (pressure1 - pressure2) * m; // L

		if(diff > 0)
		{
			temp = src->get_heat();
			mass = std::min(diff * src->get_steam_density(), src->get_steam());
		}

		else
		{
			temp = dst->get_heat();
			mass = std::min(diff * dst->get_steam_density(), dst->get_steam());
		}

		fluid_holder fh_src(*src);
		fluid_holder fh_dst(*dst);

		fh_src.add_steam(-mass, temp);
		fh_dst.add_steam(mass, temp);

//		if((pressure1 > fh_dst.get_pressure()) == (pressure2 < fh_src.get_pressure()))
		{
			break;
		}
		
		overshoots += 1;
		m *= 0.5;
	}

	if(overshoots > 0)
	{
		std::cout << "Warning: overshot " << overshoots << " times\n";
	}

	src->add_steam(-mass, temp);
	dst->add_steam(mass, temp);

	this->flow = mass / dt;
}

