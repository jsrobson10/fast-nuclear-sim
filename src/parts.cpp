
#include "parts.hpp"

#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"

using namespace sim;

reactor::coolant::vessel* parts::vessel;
reactor::reactor* parts::reactor;
coolant::valve<sim::reactor::coolant::vessel>* parts::valve;
coolant::pump<sim::reactor::coolant::vessel>* parts::pump;

void parts::init()
{
	const char* layout[] = {
		"#C#C#",
		"CFCFC",
		"#C#C#",
		"CFCFC",
		"#C#C#"
	};
	
	vessel = new reactor::coolant::vessel(8, 10, 300, sim::coolant::WATER);
	reactor = new sim::reactor::reactor(sim::reactor::builder(5, 5,
		reactor::fuel::fuel_rod(2000, 4000),
		reactor::control::control_rod(*vessel, 10000, 1),
		reactor::coolant::pipe(*vessel),
		layout));
	
	valve = new coolant::valve<reactor::coolant::vessel>(*vessel, 1, 500);
	pump = new coolant::pump<reactor::coolant::vessel>(*vessel, 1e4, 15);
}

