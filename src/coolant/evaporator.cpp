
#include "evaporator.hpp"
#include "../util/constants.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Coolant;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

Evaporator::Evaporator(Fluid type, double height, double diameter, double mass, double level) :
		height(height),
		diameter(diameter),
		FluidHolder(type, calc_cylinder(height, diameter), mass)
{
	this->level = level;
}

Evaporator::Evaporator(const Json::Value& node) :
		height(node["height"].asDouble()),
		diameter(node["diameter"].asDouble()),
		FluidHolder(node)
{
	steam_output = node["steam_output"].asDouble();
}

double Evaporator::get_steam_output()
{
	return steam_output;
}

void Evaporator::update(double dt)
{
	steam_output = steam / dt;
	steam = 0;

	double P = 10000;									// Pa
	double K = Conversions::Temperature::c_to_k(heat);	// K
	double R = Util::Constants::R;						// J/K/mol

	double n_g = air / Util::Constants::M_air;	// mol
	double V_g = (volume - level) * 0.001;		// m^3

	double n = (P * V_g) / (R * K);	// mol
	
	air = n * Util::Constants::M_air;
	update_base(dt);
}

Evaporator::operator Json::Value() const
{
	Json::Value node(FluidHolder::operator::Json::Value());

	node["height"] = height;
	node["diameter"] = diameter;
	node["steam_output"] = steam_output;
	
	return node;
}

