
#pragma once

#include <memory>

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"

namespace sim
{

struct system
{
	sim::reactor::reactor* reactor;
	sim::reactor::coolant::vessel* vessel;
	sim::coolant::valve<sim::reactor::coolant::vessel>* valve;
	sim::coolant::pump<sim::reactor::coolant::vessel>* pump;

	system();
	system(system&& o);
	system(const system& o) = delete;
	~system();

	void update(double dt);
};

};

