
#pragma once

#include <memory>
#include <json/json.h>

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "electric/grid.hpp"
#include "coolant/loop.hpp"
#include "coolant/pool.hpp"

namespace Sim
{

struct System
{
	static std::unique_ptr<System> active;

	std::string path;

	Electric::Grid grid;
	Reactor::Reactor reactor;
	Reactor::Coolant::Vessel vessel;
	Coolant::Evaporator evaporator;
	Coolant::Pool pool;
	Coolant::Pump freight_pump;
	Coolant::Sink sink;
	Coolant::Loop loop;

	double speed = 1;
	double clock = 3600 * 12;

	System(const std::string& path);
	System(const Json::Value& node, const std::string& path);
	System(const System& o) = delete;
	System(System&& o) = delete;

	void update(double dt);

	static void save();
	static void load(const std::string& path);

	operator Json::Value() const;
};

};

