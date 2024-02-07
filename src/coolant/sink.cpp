
#include "sink.hpp"

using namespace sim::coolant;

sink::sink(fluid_t type, double heat, double pressure, double steam_density) :
		heat(heat),
		pressure(pressure),
		steam_density(steam_density),
		fluid_holder(type, 2e9, 0)
{
	level = 1e9;
}

