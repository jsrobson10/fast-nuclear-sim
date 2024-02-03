
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
	
	vessel = std::make_unique<reactor::coolant::vessel>(sim::coolant::WATER, 8, 10, 300);
	reactor = std::make_unique<reactor::reactor>(sim::reactor::builder(19, 19, 1.0 / 4.0, 4, reactor::fuel::fuel_rod(0.5), vessel.get(), layout));
	condenser = std::make_unique<coolant::condenser>(sim::coolant::WATER, 8, 6, 20);
	turbine = std::make_unique<electric::turbine>(sim::coolant::WATER, 6, 3, 20);
	
	turbine_inlet_valve = std::make_unique<coolant::valve>(vessel.get(), turbine.get(), 1);
	turbine_bypass_valve = std::make_unique<coolant::valve>(vessel.get(), condenser.get(), 1);
	core_pump = std::make_unique<coolant::pump>(condenser.get(), vessel.get(), 1e6, 1, 100, 100);
}

system::system(system&& o)
{
	vessel = std::move(o.vessel);
	reactor = std::move(o.reactor);
	condenser = std::move(o.condenser);
	turbine = std::move(o.turbine);
	turbine_bypass_valve = std::move(o.turbine_bypass_valve);
	turbine_inlet_valve = std::move(o.turbine_inlet_valve);
	core_pump = std::move(o.core_pump);
}

void system::update(double dt)
{
	dt *= speed;
	vessel->update(dt);
	reactor->update(dt);
	condenser->update(dt);
	turbine->update(dt);
	turbine_inlet_valve->update(dt);
	turbine_bypass_valve->update(dt);
//	core_pump->update(dt);
}

