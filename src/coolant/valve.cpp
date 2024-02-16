
#include "valve.hpp"
#include "../conversions/temperature.hpp"
#include "../util/constants.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Coolant;

Valve::Valve(FluidHolder* src, FluidHolder* dst, double state, double max) : src(src), dst(dst), max(max)
{
	this->state = state;
}

void Valve::add_open_speed(double v)
{
	speed += v;
}

void Valve::clear_open_speed()
{
	speed = 0;
}

void Valve::toggle_auto()
{
	set_auto(!auto_on);
}

void Valve::set_auto(bool state)
{
	if(state)
	{
		auto_th = src->get_heat();
	}

	else
	{
		auto_th = 0;
	}

	auto_on = state;
	speed = 0;
}

void Valve::update(double dt)
{
	if(auto_on)
	{
		state -= pid.calculate(dt, auto_th, src->get_heat()) * dt;
		auto_th += speed * dt * 100;
	}

	else
	{
		state += speed * dt;
	}

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

		mol = FluidHolder::calc_pressure_mol(src->get_heat_k(), src->get_gas_volume(), pressure1 - remove);
		
		mass_a = src->get_air() - mol / Util::Constants::M_air;
		mass_s = src->get_steam() - src->fluid.mol_to_g(mol);
	}

	else
	{
		ratio_a = dst->get_air() / dst->get_gas();
		ratio_s = dst->get_steam() / dst->get_gas();

		mol = FluidHolder::calc_pressure_mol(dst->get_heat_k(), dst->get_gas_volume(), pressure2 - remove);

		mass_a = dst->get_air() - mol / Util::Constants::M_air;
		mass_s = dst->get_steam() - dst->fluid.mol_to_g(mol);
	}

	mass_a *= ratio_a;
	mass_s *= ratio_s;

	double heat1 = src->get_heat(); // C
	double heat2 = dst->get_heat();

	src->add_gas(-mass_s, mass_a, heat2);
	dst->add_gas(mass_s, mass_a, heat1);

	this->flow = (mass_s + mass_a) / dt;
}

Valve::Valve(const Json::Value& node, FluidHolder* src, FluidHolder* dst) : src(src), dst(dst), max(node["max"].asDouble()), pid(node["pid"])
{
	state = node["state"].asDouble();
	flow = node["flow"].asDouble();
	auto_th = node["auto_th"].asDouble();
	auto_on = node["auto_on"].asBool();
}

Valve::operator Json::Value() const
{
	Json::Value node;

	node["max"] = max;
	node["state"] = state;
	node["flow"] = flow;
	node["auto_th"] = auto_th;
	node["auto_on"] = auto_on;
	node["pid"] = pid;

	return node;
}

