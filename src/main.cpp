
#include "reactor.hpp"
#include "control/control_rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "coolant/pipe.hpp"
#include "display.hpp"

#include <sstream>
#include <unistd.h>
#include <curses.h>

int main()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	
	sim::reactor<2, 2> reactor({
		new sim::fuel::fuel_rod(100, 400),		new sim::fuel::fuel_rod(100, 400),
		new sim::control::control_rod(1000),	new sim::coolant::pipe()
	});

	((sim::control::control_rod*)reactor.rods[0][1])->set_reactivity(0.99);

	for(;;)
	{
		for(int i = 0; i < 1e3; i++)
		{
			reactor.update(1e-3);
		}

		erase();
		display::draw_text(1, 0, "Reactor Core:");

		const int X = 3, Y = 4;
		const int W = 32, H = 8;

		for(int x = 0; x < reactor.width; x++)
		for(int y = 0; y < reactor.height; y++)
		{
			std::stringstream ss;
			ss << *reactor.rods[x][y];
			
			int px = X + (H - 1) * y;
			int py = Y + (W - 1) * x;

			display::draw_text(px + 1, py + 2, ss.str().c_str());
			display::draw_box(px, py, H, W);
		}

		refresh();
	}

	return 0;
}

