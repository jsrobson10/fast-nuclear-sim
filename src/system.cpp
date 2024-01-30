
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
	
	vessel = std::make_unique<reactor::coolant::vessel>(8, 10, 300, sim::coolant::WATER);
	reactor = std::make_unique<sim::reactor::reactor>(sim::reactor::builder(19, 19, 0.25, 8,
		reactor::fuel::fuel_rod(0.5),
		reactor::control::boron_rod(*vessel.get(), 1),
		*vessel.get(), layout));
	
	valve = std::make_unique<coolant::valve<reactor::coolant::vessel>>(*vessel.get(), 1, 500);
	pump = std::make_unique<coolant::pump<reactor::coolant::vessel>>(*vessel.get(), 1e4, 15);
}

system::system(system&& o)
{
	vessel = std::move(o.vessel);
	reactor = std::move(o.reactor);
	valve = std::move(o.valve);
	pump = std::move(o.pump);
}

void system::update(double dt)
{
	dt *= 10;
	vessel->update(dt);
	reactor->update(dt);
	valve->update(dt);
	pump->update(dt);
}

