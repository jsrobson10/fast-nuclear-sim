
#include "system.hpp"

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"

using namespace sim;

sim::system system::active;

system::system()
{
	const char* layout[] = {
		"      C C C C      ",
		"   C CFCFCFCFC C   ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		"   C CFCFCFCFC C   ",
		"      C C C C      "
	};
	
	vessel = std::make_unique<reactor::coolant::vessel>(sim::coolant::WATER, 8, 10, 6e6, 5e5);
	reactor = std::make_unique<reactor::reactor>(sim::reactor::builder(19, 19, 1.0 / 4.0, 4, reactor::fuel::fuel_rod(0.5), vessel.get(), layout));
	condenser = std::make_unique<coolant::condenser>(sim::coolant::WATER, 6, 4, 3e6, 30000);
	turbine = std::make_unique<electric::turbine>(sim::coolant::WATER, condenser.get(), 6, 3, 2e6);
	
	sink = std::make_unique<coolant::sink>(sim::coolant::WATER, 11, 0, 0);
	evaporator = std::make_unique<coolant::evaporator>(sim::coolant::WATER, 2, 30, 0, 1000);
	condenser_secondary = std::make_unique<coolant::condenser_secondary>(condenser.get(), evaporator.get(), 1000);
	
	turbine_inlet_valve = std::make_unique<coolant::valve>(vessel.get(), turbine.get(), 0, 0.5);
	turbine_bypass_valve = std::make_unique<coolant::valve>(vessel.get(), condenser.get(), 0, 0.5);

	primary_pump = std::make_unique<coolant::pump>(condenser.get(), vessel.get(), 1e5, 1, 1e5, 0.1, 10, coolant::pump::mode_t::SRC, 35000);
	secondary_pump = std::make_unique<coolant::pump>(evaporator.get(), condenser_secondary.get(), 1e5, 1, 1e4, 0.1, 1, coolant::pump::mode_t::NONE, 0);
	freight_pump = std::make_unique<coolant::pump>(sink.get(), evaporator.get(), 1e5, 1, 1e4, 0.1, 10, coolant::pump::mode_t::DST, 1e6);
}

system::system(system&& o)
{
	vessel = std::move(o.vessel);
	reactor = std::move(o.reactor);
	condenser = std::move(o.condenser);
	turbine = std::move(o.turbine);

	sink = std::move(o.sink);
	condenser_secondary = std::move(o.condenser_secondary);
	evaporator = std::move(o.evaporator);

	turbine_bypass_valve = std::move(o.turbine_bypass_valve);
	turbine_inlet_valve = std::move(o.turbine_inlet_valve);

	primary_pump = std::move(o.primary_pump);
	secondary_pump = std::move(o.secondary_pump);
	freight_pump = std::move(o.freight_pump);
}

void system::update(double dt)
{
	dt *= speed;
	
	turbine_inlet_valve->update(dt);
	turbine_bypass_valve->update(dt);
	primary_pump->update(dt);
	secondary_pump->update(dt);
	freight_pump->update(dt);
	reactor->update(dt);

	vessel->update(dt);
	turbine->update(dt);
	condenser->update(dt);
	evaporator->update(dt);
	condenser_secondary->update(dt);
}

