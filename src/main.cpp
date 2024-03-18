
#include <random>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cfenv>

#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"

#include "graphics/input/focus.hpp"

#include "graphics/window.hpp"
#include "graphics/camera.hpp"

#include "util/time.hpp"
#include "system.hpp"
#include "tests.hpp"

using namespace Sim;

int main()
{
#ifndef _WIN32
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif
	
//	Tests::run();
//	return 0;

	Graphics::Window::create();

	long clock = Util::Time::get_now();
	double at = 0;

	while(!Graphics::Window::should_close())
	{
		long now = Util::Time::get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;

		clock += passed;
		at += dt * Sim::System::active->speed;
		Sim::System::active->update(dt);

		Graphics::Camera::update(dt);
		Graphics::Window::update(dt);
		Graphics::Focus::update(dt);
		Graphics::Window::render();
	}

	Graphics::Window::destroy();
}

