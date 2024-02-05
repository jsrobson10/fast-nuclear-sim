
#pragma once

#include <memory>

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"
#include "coolant/condenser.hpp"
#include "electric/turbine.hpp"
#include "graphics/mesh/mesh.hpp"

namespace sim
{

struct system
{
	static system active;

	std::unique_ptr<sim::reactor::reactor> reactor;
	std::unique_ptr<sim::reactor::coolant::vessel> vessel;
	std::unique_ptr<sim::coolant::condenser> condenser;
	std::unique_ptr<sim::electric::turbine> turbine;
	std::unique_ptr<sim::coolant::pump> primary_pump;
	std::unique_ptr<sim::coolant::valve> turbine_bypass_valve;
	std::unique_ptr<sim::coolant::valve> turbine_inlet_valve;
	sim::graphics::mesh scene;
	double speed = 1;

	system();
	system(system&& o);
	system(const system& o) = delete;

	void update(double dt);
};

};

