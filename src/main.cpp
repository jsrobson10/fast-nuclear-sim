
#include <sys/time.h>

#include <random>
#include <sstream>
#include <cmath>

#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid_t.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"

#include "graphics/mesh/mesh.hpp"

#include "graphics/window.hpp"
#include "graphics/camera.hpp"

#include "system.hpp"

using namespace sim;

unsigned long get_now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_sec * 1000000 + tv.tv_usec;
}

int main()
{
	sim::system sys;
	graphics::window::create(sys);

	long clock = get_now();

	while(!graphics::window::should_close())
	{
		long now = get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;
		clock += passed;
		
		sys.update(dt);
		graphics::camera::update(sys, dt);
		graphics::window::loop(sys);
	}

	graphics::window::destroy();
}

