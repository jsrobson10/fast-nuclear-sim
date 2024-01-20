
#include "graphics/window.hpp"

using namespace sim;

int main()
{
	graphics::window::create();

	while(!graphics::window::should_close())
	{
		graphics::window::loop();
	}

	graphics::window::destroy();
}

