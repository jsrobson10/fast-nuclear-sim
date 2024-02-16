
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cfenv>

#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"

#include "graphics/mesh/mesh.hpp"
#include "graphics/input/focus.hpp"

#include "graphics/window.hpp"
#include "graphics/camera.hpp"

#include "util/time.hpp"
#include "system.hpp"
#include "tests.hpp"

using namespace sim;

int main()
{
#ifndef _WIN32
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif
	
//	tests::run();
//	return 0;

	graphics::window::create();

	long clock = util::time::get_now();
	double at = 0;

	while(!graphics::window::should_close())
	{
		long now = util::time::get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;
		clock += passed;
		at += dt * sim::System::active.speed;
		
		sim::System::active.update(dt);

		graphics::camera::update(dt);
		graphics::window::update(dt);
		graphics::focus::update(dt);
		graphics::window::render();
	}

	graphics::window::destroy();
}

