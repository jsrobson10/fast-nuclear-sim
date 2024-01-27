
#pragma once

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"

namespace sim
{

struct system
{
	sim::reactor::reactor reactor;
	sim::reactor::coolant::vessel vessel;
	sim::coolant::valve<sim::reactor::coolant::vessel> valve;
	sim::coolant::pump<sim::reactor::coolant::vessel> pump;

	void update(double dt);
	static system generate();
};

};

