
#include "condenser_secondary.hpp"
#include "../conversions/temperature.hpp"
#include "../util/constants.hpp"
#include "../system.hpp"

using namespace sim::coolant;

condenser_secondary::condenser_secondary(condenser* primary, evaporator* source) :
		primary(primary), source(source), fluid_holder(primary->fluid, 0, 0)
{
	
}

double condenser_secondary::add_fluid(double amount, double heat)
{
	heat = primary->add_heat(fluid.l_to_g(amount), heat);
	return source->add_fluid(amount, heat);
}

