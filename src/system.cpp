
#include "system.hpp"

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"

using namespace sim;

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
	
	vessel = new reactor::coolant::vessel(8, 10, 300, sim::coolant::WATER);
	reactor = new sim::reactor::reactor(sim::reactor::builder(19, 19, 0.25, 8,
		reactor::fuel::fuel_rod(0.5),
		reactor::control::boron_rod(*vessel, 1),
		*vessel, layout));
	
	valve = new coolant::valve<reactor::coolant::vessel>(*vessel, 1, 500);
	pump = new coolant::pump<reactor::coolant::vessel>(*vessel, 1e4, 15);

	scene.load_model("../assets/model", "scene_collisions.stl");
}

system::system(system&& o)
{
	vessel = o.vessel;
	reactor = o.reactor;
	valve = o.valve;
	pump = o.pump;

	o.vessel = nullptr;
	o.reactor = nullptr;
	o.valve = nullptr;
	o.pump = nullptr;
}

system::~system()
{
	if(vessel != nullptr) delete vessel;
	if(reactor != nullptr) delete reactor;
	if(valve != nullptr) delete valve;
	if(pump != nullptr) delete pump;
}

void system::update(double dt)
{
	dt *= 10;
	vessel->update(dt);
	reactor->update(dt);
	valve->update(dt);
	pump->update(dt);
}

