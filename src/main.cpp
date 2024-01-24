
#include <sys/time.h>

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

