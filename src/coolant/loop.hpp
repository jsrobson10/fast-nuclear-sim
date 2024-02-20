
#pragma once

#include "pump.hpp"
#include "valve.hpp"
#include "condenser.hpp"
#include "condenser_secondary.hpp"
#include "evaporator.hpp"
#include "sink.hpp"
#include "fluid.hpp"
#include "../reactor/coolant/vessel.hpp"
#include "../electric/turbine.hpp"
#include "../electric/generator.hpp"
#include "../electric/grid.hpp"

#include <json/json.h>

namespace Sim::Coolant
{

class Loop
{
public:

	Coolant::Condenser condenser;
	Coolant::CondenserSecondary condenser_secondary;
	Electric::Turbine turbine;
	Electric::Generator generator;
	Coolant::Pump primary_pump;
	Coolant::Pump secondary_pump;
	Coolant::Valve turbine_bypass_valve;
	Coolant::Valve turbine_inlet_valve;

	Loop(Reactor::Coolant::Vessel* vessel, Evaporator* evaporator, Electric::Grid* grid);
	Loop(const Json::Value& node, Reactor::Coolant::Vessel* vessel, Evaporator* evaporator, Electric::Grid* grid);
	Loop(const Loop& o) = delete;
	Loop(Loop&& o) = delete;

	operator Json::Value() const;

	void update(double dt);
};

};

