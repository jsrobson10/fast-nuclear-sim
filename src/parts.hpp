
#pragma once

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"

namespace sim::parts
{

extern sim::reactor::coolant::vessel* vessel;
extern sim::reactor::reactor* reactor;
extern sim::coolant::valve<sim::reactor::coolant::vessel>* valve;
extern sim::coolant::pump<sim::reactor::coolant::vessel>* pump;

void init();

};

