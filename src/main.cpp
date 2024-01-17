
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
#include <sys/time.h>

unsigned long get_now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_sec * 1000000 + tv.tv_usec;
}

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
		sim::reactor::fuel::fuel_rod(2000, 4000),
		sim::reactor::control::control_rod(10000, 1),
		sim::reactor::coolant::pipe(vessel), {
			"#  ##",
			"#FCF ",
			" C C ",
			" FCF#",
			"##  #"
		});

	double secs = 0;
	long clock = get_now();
	double speed = 10000;
	int framerate = 100;
	int extra = 100;

	speed /= extra;

	for(;;)
	{

		std::stringstream ss;
		ss << "Reactor Core\n\n";

		{
			long mins = secs / 60;
			long hours = mins / 60;
			long days = hours / 24;
			long years = days / 365;
			double s = fmod(secs, 60);

			mins %= 60;
			hours %= 24;
			days %= 365;

			ss << "Time:\n";

			if(years > 0) goto years;
			if(days > 0)  goto days;
			if(hours > 0) goto hours;
			if(mins > 0)  goto mins;
			goto secs;

years:		ss << years << "y ";
days:		ss << days << "d ";
hours:		ss << hours << "h ";
mins:		ss << mins << "m ";
secs:		ss << s << "s\n\n";
		}

		for(int i = 0; i < extra; i++)
		{
			reactor.update(speed / framerate);
			vessel.update();
			secs += speed / framerate;
		}
		
		ss << "Vessel\n" << vessel << "\n";

		erase();
		display::draw_text(1, 0, ss.str().c_str());

		const int X = 1, Y = 30;
		const int W = 36, H = 11;

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

		long now = get_now();

		while(clock + 1e6 / framerate > now)
		{
			usleep(clock + 1e6 / framerate - now);
			now = get_now();
		}

		clock += 1e6 / framerate;
	}

	return 0;
}

