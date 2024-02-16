
#include "sink.hpp"

using namespace sim::coolant;

Sink::Sink(Fluid type, double heat, double pressure, double steam_density) :
		heat(heat),
		pressure(pressure),
		steam_density(steam_density),
		FluidHolder(type, 2e9, 0)
{
	level = 1e9;
}

