
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "control/control_rod.hpp"
#include "control/graphite_rod.hpp"
#include "coolant/pipe.hpp"
#include "coolant/heater.hpp"
#include "reactor.hpp"

namespace sim::reactor
{

reactor builder(const int W, const int H, fuel::fuel_rod fr, control::control_rod cr, coolant::pipe p, const char** lines);

};

