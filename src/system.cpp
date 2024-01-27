
#include "system.hpp"

#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"

using namespace sim;

sim::system sim::system::generate()
{
	const char* layout[] = {
		"#C#C#",
		"CFCFC",
		"#C#C#",
		"CFCFC",
		"#C#C#"
	};
	
	reactor::coolant::vessel vessel(8, 10, 300, sim::coolant::WATER);
	sim::reactor::reactor reactor(sim::reactor::builder(5, 5,
		reactor::fuel::fuel_rod(2000, 4000),
		reactor::control::control_rod(vessel, 10000, 1),
		reactor::coolant::pipe(vessel),
		layout));
	
	coolant::valve<reactor::coolant::vessel> valve(vessel, 1, 500);
	coolant::pump<reactor::coolant::vessel> pump(vessel, 1e4, 15);

	return {
		.reactor = reactor,
		.vessel = vessel,
		.valve = valve,
		.pump = pump
	};
}

void system::update(double dt)
{
	vessel.update(dt);
	reactor.update(dt);
	valve.update(dt);
	pump.update(dt);
}

