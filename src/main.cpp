
#include <sys/time.h>

#include <random>
#include <sstream>
#include <cmath>

#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid_t.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"

#include "graphics/window.hpp"
#include "graphics/camera.hpp"

using namespace sim;

unsigned long get_now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_sec * 1000000 + tv.tv_usec;
}

int main()
{
	std::random_device rd;
	std::mt19937 rand(rd());
	
	sim::reactor::coolant::vessel vessel(8, 10, 300, sim::coolant::WATER);
	sim::reactor::reactor<5, 5> reactor = sim::reactor::builder<5, 5>(
		sim::reactor::fuel::fuel_rod(2000, 4000),
		sim::reactor::control::control_rod(vessel, 10000, 1),
		sim::reactor::coolant::pipe(vessel), {
			"#C#C#",
			"CFCFC",
			"#C#C#",
			"CFCFC",
			"#C#C#"
		});
	
	sim::coolant::valve<sim::reactor::coolant::vessel> valve(vessel, 1, 500);
	sim::coolant::pump<sim::reactor::coolant::vessel> pump(vessel, 1e4, 15);

	graphics::window::create();

	long clock = get_now();

	while(!graphics::window::should_close())
	{
		long now = get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;
		clock += passed;
		
		std::stringstream ss;

		reactor.update(rand, dt);
		pump.update(dt);
		valve.update(dt);
		vessel.update(dt);
		
		ss << "Reactor Vessel\n\n" << vessel;

		graphics::camera::update(dt);
		graphics::window::loop(ss.str().c_str());
	}

	graphics::window::destroy();
}

