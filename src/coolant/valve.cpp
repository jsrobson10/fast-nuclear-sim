
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
	
	double m = max * state;
	double diff = (pressure1 - pressure2);
	double remove = diff - diff * std::pow(1 - m, dt);
	
	double mol, mass;

	if(remove < 0)
	{
		mol = fluid_holder::calc_pressure_mol(src->get_heat_k(), src->get_steam_volume(), pressure1 - remove);
		mass = src->get_steam() - src->fluid.mol_to_g(mol);
	}

	else
	{
		mol = fluid_holder::calc_pressure_mol(dst->get_heat_k(), dst->get_steam_volume(), pressure2 - remove);
		mass = dst->get_steam() - dst->fluid.mol_to_g(mol);
	}

	double heat1 = src->get_heat(); // C
	double heat2 = dst->get_heat();

	src->add_steam(-mass, heat2);
	dst->add_steam(mass, heat1);

	this->flow = mass / dt;
}

