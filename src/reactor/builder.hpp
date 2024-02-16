
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "coolant/vessel.hpp"
#include "reactor.hpp"

#include <json/json.h>

namespace Sim::Reactor
{

Reactor Builder(const int W, const int H, const double CW, const double CH, Fuel::FuelRod fr, Coolant::Vessel* v, const char** lines);
std::unique_ptr<Rod> load_rod(const Json::Value& node, Coolant::Vessel* v);

};

