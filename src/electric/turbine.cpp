
#include "turbine.hpp"
#include "../system.hpp"
#include "../util/math.hpp"
#include "../util/streams.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Electric;

Turbine::Turbine(Coolant::Condenser* condenser) :
		condenser(condenser), Sim::Coolant::FluidHolder(condenser->fluid, 1, 0)
{
	
}

void Turbine::add_gas(double steam, double air, double t)
{
	energy_output += (steam + air) * fluid.jPg; // J
	condenser->add_gas(steam, air, t);
}

void Turbine::update(double dt)
{

}

double Turbine::extract_energy()
{
	double e = energy_output;
	energy_output = 0;
	return e;
}

