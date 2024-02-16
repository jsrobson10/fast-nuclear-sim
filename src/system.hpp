
#pragma once

#include <memory>
#include <json/json.h>

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"
#include "coolant/condenser.hpp"
#include "coolant/condenser_secondary.hpp"
#include "coolant/evaporator.hpp"
#include "coolant/sink.hpp"
#include "electric/turbine.hpp"

namespace Sim
{

struct System
{
	static System active;

	std::unique_ptr<Sim::Reactor::Reactor> reactor;
	std::unique_ptr<Sim::Reactor::Coolant::Vessel> vessel;

	std::unique_ptr<Sim::Coolant::Sink> sink;
	std::unique_ptr<Sim::Coolant::Condenser> condenser;
	std::unique_ptr<Sim::Coolant::CondenserSecondary> condenser_secondary;
	std::unique_ptr<Sim::Coolant::Evaporator> evaporator;
	std::unique_ptr<Sim::Electric::Turbine> turbine;

	std::unique_ptr<Sim::Coolant::Pump> primary_pump;
	std::unique_ptr<Sim::Coolant::Pump> secondary_pump;
	std::unique_ptr<Sim::Coolant::Pump> freight_pump;

	std::unique_ptr<Sim::Coolant::Valve> turbine_bypass_valve;
	std::unique_ptr<Sim::Coolant::Valve> turbine_inlet_valve;

	double speed = 1;
	double clock = 3600 * 12;

	System();
	System(const Json::Value& node);

	void update(double dt);

	static void save();
	static void load();

	operator Json::Value() const;
};

};

