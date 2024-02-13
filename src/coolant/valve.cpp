
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

	if(src->get_gas_volume() == 0 || dst->get_gas_volume() == 0 || (src->get_gas() == 0 && dst->get_gas() == 0))
	{
		flow = 0;
		return;
	}

	double pressure1 = src->get_pressure(); // Pa
	double pressure2 = dst->get_pressure();
	
	double m = max * state;
	double diff = (pressure1 - pressure2);
	double remove = diff - diff * std::pow(1 - m, dt);

	double ratio_a, ratio_s;
	double mass_a, mass_s;
	double mol;

	if(remove < 0)
	{
		ratio_a = src->get_air() / src->get_gas();
		ratio_s = src->get_steam() / src->get_gas();

		mol = fluid_holder::calc_pressure_mol(src->get_heat_k(), src->get_gas_volume(), pressure1 - remove);
		
		mass_a = src->get_air() - mol / constants::M_air;
		mass_s = src->get_steam() - src->fluid.mol_to_g(mol);
	}

	else
	{
		ratio_a = dst->get_air() / dst->get_gas();
		ratio_s = dst->get_steam() / dst->get_gas();

		mol = fluid_holder::calc_pressure_mol(dst->get_heat_k(), dst->get_gas_volume(), pressure2 - remove);

		mass_a = dst->get_air() - mol / constants::M_air;
		mass_s = dst->get_steam() - dst->fluid.mol_to_g(mol);
	}

	mass_a *= ratio_a;
	mass_s *= ratio_s;

	double heat1 = src->get_heat(); // C
	double heat2 = dst->get_heat();

	src->add_gas(-mass_s, mass_a, heat2, 0);
	dst->add_gas(mass_s, mass_a, heat1, 0);

	this->flow = (mass_s + mass_a) / dt;
}

valve::operator Json::Value() const
{
	Json::Value node;

	node["max"] = max;
	node["speed"] = speed;
	node["state"] = state;
	node["flow"] = flow;

	return node;
}

