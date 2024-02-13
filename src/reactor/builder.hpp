
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "coolant/vessel.hpp"
#include "reactor.hpp"

#include <json/json.h>

namespace sim::reactor
{

reactor builder(const int W, const int H, const double CW, const double CH, fuel::fuel_rod fr, coolant::vessel* v, const char** lines);
std::unique_ptr<rod> load_rod(const Json::Value& node, coolant::vessel* v);

};

