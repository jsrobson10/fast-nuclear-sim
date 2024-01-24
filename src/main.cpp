
#include <sys/time.h>

#include <random>
#include <iostream>
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

	graphics::window::create();

	long clock = get_now();

	while(!graphics::window::should_close())
	{
		long now = get_now();
		long passed = now - clock;
		double dt = (double)passed / 1e6;
		clock += passed;

		graphics::camera::update(dt);
		graphics::window::loop();
	}

	graphics::window::destroy();
}

