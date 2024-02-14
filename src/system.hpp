
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

struct system
{
	static system active;

	std::unique_ptr<sim::reactor::reactor> reactor;
	std::unique_ptr<sim::reactor::coolant::vessel> vessel;

	std::unique_ptr<sim::coolant::sink> sink;
	std::unique_ptr<sim::coolant::condenser> condenser;
	std::unique_ptr<sim::coolant::condenser_secondary> condenser_secondary;
	std::unique_ptr<sim::coolant::evaporator> evaporator;
	std::unique_ptr<sim::electric::turbine> turbine;

	std::unique_ptr<sim::coolant::pump> primary_pump;
	std::unique_ptr<sim::coolant::pump> secondary_pump;
	std::unique_ptr<sim::coolant::pump> freight_pump;

	std::unique_ptr<sim::coolant::valve> turbine_bypass_valve;
	std::unique_ptr<sim::coolant::valve> turbine_inlet_valve;

	double speed = 1;
	double clock = 0;

	system();
	system(const Json::Value& node);

	void update(double dt);

	static void save();
	static void load();

	operator Json::Value() const;
};

};

