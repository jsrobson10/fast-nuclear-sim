
#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
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
	
	sim::reactor::reactor<5, 5> reactor = sim::reactor::builder<5, 5>(
		sim::reactor::fuel::fuel_rod(100, 400),
		sim::reactor::control::control_rod(1000, 0.1),
		sim::reactor::coolant::pipe(), {
			" PPP ",
			"PFCFP",
			"PCPCP",
			"PFCFP",
			" PPP "
		});

	double secs = 0;

	for(;;)
	{
		reactor.update(0.01);

		std::stringstream ss;
		ss << "Reactor Core: " << secs << " s\n";

		secs += 0.01;

		erase();
		display::draw_text(1, 0, ss.str().c_str());

		const int X = 3, Y = 4;
		const int W = 32, H = 8;

		for(int x = 0; x < reactor.width; x++)
		for(int y = 0; y < reactor.height; y++)
		{
			std::stringstream ss;
			sim::reactor::rod* r = reactor.rods[x][y];
			ss << *r;
			
			int px = X + (H - 1) * y;
			int py = Y + (W - 1) * x;

			display::draw_text(px + 1, py + 2, ss.str().c_str());
			display::draw_box(px, py, H, W);

			if(r->should_select() && x == reactor.cursor_x && y == reactor.cursor_y)
			{
				display::draw_text(px + 1, py + W - 5, "[ ]");
			}

			if(r->is_selected())
			{
				display::draw_text(px + 1, py + W - 4, "#");
			}
		}

		refresh();

		int c = getch();

		switch(c)
		{
		case KEY_LEFT:
			reactor.move_cursor(-1);
			break;
		case KEY_RIGHT:
			reactor.move_cursor(1);
			break;
		case KEY_UP:
			reactor.update_selected(0.01);
			break;
		case KEY_DOWN:
			reactor.update_selected(-0.01);
			break;
		case ' ':
			reactor.toggle_selected();
			break;
		}
	}

	return 0;
}

