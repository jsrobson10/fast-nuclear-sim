
#include "condenser_secondary.hpp"
#include "../conversions/temperature.hpp"
#include "../util/constants.hpp"
#include "../system.hpp"

using namespace Sim::Coolant;

CondenserSecondary::CondenserSecondary(Condenser* primary, Evaporator* source) :
		primary(primary), source(source), FluidHolder(primary->fluid, 1, 0)
{
	
}

double CondenserSecondary::add_fluid(double amount, double heat)
{
	heat = primary->add_heat(fluid.l_to_g(amount), heat);
	return source->add_fluid(amount, heat);
}

void CondenserSecondary::update(double dt)
{
	heat = primary->add_heat(fluid.l_to_g(level), heat);
}

