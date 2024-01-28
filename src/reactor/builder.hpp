
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "control/boron_rod.hpp"
#include "control/graphite_rod.hpp"
#include "coolant/pipe.hpp"
#include "coolant/heater.hpp"
#include "coolant/vessel.hpp"
#include "reactor.hpp"

namespace sim::reactor
{

reactor builder(const int W, const int H, const double CW, const double CH, fuel::fuel_rod fr, control::boron_rod br, coolant::vessel& v, const char** lines);

};

