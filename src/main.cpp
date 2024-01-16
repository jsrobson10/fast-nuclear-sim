
#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid_t.hpp"
#include "display.hpp"

#include <cmath>
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
	
	sim::reactor::coolant::vessel vessel(200, 400, sim::coolant::WATER);
	sim::reactor::reactor<5, 5> reactor = sim::reactor::builder<5, 5>(
		sim::reactor::fuel::fuel_rod(1000, 4000),
		sim::reactor::control::control_rod(10000, 1),
		sim::reactor::coolant::pipe(vessel), {
			"#   #",
			" FCF ",
			" C C ",
			" FCF ",
			"#   #"
		});

	double secs = 0;

	for(;;)
	{

		std::stringstream ss;
		ss << "Reactor Core\n\n";

		{
			long mins = secs / 60;
			double s = fmod(secs, 60);

			long hours = mins / 60;
			mins %= 60;

			long days = hours / 24;
			hours %= 24;

			long years = days / 365;
			days %= 365;

			ss << "Time:\n";

			if(years > 0) ss << years << "y ";
			if(days > 0) ss << days << "d ";
			if(hours > 0) ss << hours << "h ";
			if(mins > 0) ss << mins << "m ";
			
			ss << s << "s\n\n";
		}

		reactor.update(1);
		vessel.update();
		secs += 1;
		
		ss << "Vessel\n" << vessel << "\n";

		erase();
		display::draw_text(1, 0, ss.str().c_str());

		const int X = 2, Y = 40;
		const int W = 32, H = 10;

		for(int x = 0; x < reactor.width; x++)
		for(int y = 0; y < reactor.height; y++)
		{
			sim::reactor::rod* r = reactor.rods[x][y];

			if(!r->should_display())
			{
				continue;
			}

			std::stringstream ss;
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

