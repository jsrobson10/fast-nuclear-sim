
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "coolant/vessel.hpp"
#include "reactor.hpp"

#include <json/json.h>

namespace sim::reactor
{

Reactor builder(const int W, const int H, const double CW, const double CH, fuel::FuelRod fr, coolant::Vessel* v, const char** lines);
std::unique_ptr<Rod> load_rod(const Json::Value& node, coolant::Vessel* v);

};

