
#include "condenser_secondary.hpp"

using namespace sim::coolant;

condenser_secondary::condenser_secondary(condenser* primary, double volume, double level) :
		primary(primary), fluid_holder(primary->fluid, volume, 0)
{
	this->level = level;
}

void condenser_secondary::update(double dt)
{
	((fluid_holder*)this)->update(dt);
	heat = primary->add_heat(get_thermal_mass(), heat);
}

