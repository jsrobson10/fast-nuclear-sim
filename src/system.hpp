
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

namespace sim
{

struct System
{
	static System active;

	std::unique_ptr<sim::reactor::Reactor> reactor;
	std::unique_ptr<sim::reactor::coolant::Vessel> vessel;

	std::unique_ptr<sim::coolant::Sink> sink;
	std::unique_ptr<sim::coolant::Condenser> condenser;
	std::unique_ptr<sim::coolant::CondenserSecondary> condenser_secondary;
	std::unique_ptr<sim::coolant::Evaporator> evaporator;
	std::unique_ptr<sim::electric::Turbine> turbine;

	std::unique_ptr<sim::coolant::Pump> primary_pump;
	std::unique_ptr<sim::coolant::Pump> secondary_pump;
	std::unique_ptr<sim::coolant::Pump> freight_pump;

	std::unique_ptr<sim::coolant::Valve> turbine_bypass_valve;
	std::unique_ptr<sim::coolant::Valve> turbine_inlet_valve;

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

