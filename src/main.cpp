
#include <sys/time.h>

#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cfenv>

#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid_t.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"

#include "graphics/mesh/mesh.hpp"
#include "graphics/input/focus.hpp"

#include "graphics/window.hpp"
#include "graphics/camera.hpp"

#include "system.hpp"
#include "tests.hpp"

using namespace sim;

unsigned long get_now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_sec * 1000000 + tv.tv_usec;
}

int main()
{
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
	
//	tests::run();
//	return 0;

	graphics::window::create();

	long clock = get_now();
	double at = 0;

	while(!graphics::window::should_close())
	{
		long now = get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;
		clock += passed;
		at += dt * sim::system::active.speed;
		
		sim::system::active.update(dt);

		graphics::camera::update(dt);
		graphics::window::update(dt);
		graphics::focus::update(dt);
		graphics::window::render();
	}

	graphics::window::destroy();
}

