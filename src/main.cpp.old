
#include "reactor/builder.hpp"
#include "reactor/control/control_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "reactor/coolant/vessel.hpp"
#include "coolant/fluid_t.hpp"
#include "coolant/valve.hpp"
#include "coolant/pump.hpp"
#include "display.hpp"

#include <cmath>
#include <sstream>
#include <unistd.h>
#include <curses.h>
#include <sys/time.h>

const bool do_graphics = true;
const bool do_clock = true;

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
	
	if(do_graphics)
	{
		initscr();
		cbreak();
		noecho();
		keypad(stdscr, TRUE);
		nodelay(stdscr, TRUE);
		curs_set(0);
	}
	
	sim::reactor::coolant::vessel vessel(200, 400, sim::coolant::WATER);
	sim::reactor::reactor<5, 5> reactor = sim::reactor::builder<5, 5>(
		sim::reactor::fuel::fuel_rod(2000, 4000),
		sim::reactor::control::control_rod(vessel, 10000, 1),
		sim::reactor::coolant::pipe(vessel), {
			"#C#C#",
			"CFCFC",
			"#C#C#",
			"CFCFC",
			"#C#C#"
		});

	sim::coolant::valve<sim::reactor::coolant::vessel> valve(vessel, 1, 500);
	sim::coolant::pump<sim::reactor::coolant::vessel> pump(vessel, 1e4, 15);

	double secs = 0;
	long clock = get_now();
	double speed = 1;
	int framerate = 100;
	int steps_extra = 1;

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
secs:		ss << s << "s\n";

			ss << "Speed: " << speed << "x\n\n";
		}

		for(int i = 0; i < steps_extra; i++)
		{
			double dt = speed / framerate / steps_extra;
			reactor.update(rand, dt);
			pump.update(dt);
			valve.update(dt);
			vessel.update();
			secs += dt;
		}
		
		ss << "Vessel\n" << vessel << "\n";
		ss << "Steam Valve\n" << valve << "\n";
		ss << "Coolant Pump\n" << pump << "\n";

		if(do_graphics)
		{
			erase();
			display::draw_text(1, 0, ss.str().c_str());
		}

		const int X = 1, Y = 30;
		const int W = 36, H = 11;

		for(int x = 0; x < reactor.width; x++)
		for(int y = 0; y < reactor.height; y++)
		{
			int id = y * reactor.width + x;
			sim::reactor::rod* r = reactor.rods[id];

			if(!r->should_display())
			{
				continue;
			}

			std::stringstream ss;
			ss << *r;
			
			int px = X + (H - 1) * y;
			int py = Y + (W - 1) * x;

			if(do_graphics)
			{
				display::draw_text(px + 1, py + 2, ss.str().c_str());
				display::draw_box(px, py, H, W);
			}

			if(do_graphics && r->should_select() && id == reactor.cursor)
			{
				display::draw_text(px + 1, py + W - 5, "[ ]");
			}

			if(do_graphics && r->is_selected())
			{
				display::draw_text(px + 1, py + W - 4, "#");
			}
		}

		int c = 0;

		if(do_graphics)
		{
			refresh();
			c = getch();
		}

		switch(c)
		{
		case KEY_LEFT:
			reactor.move_cursor(-1);
			break;
		case KEY_RIGHT:
			reactor.move_cursor(1);
			break;
		case KEY_UP:
			reactor.update_selected(1);
			break;
		case KEY_DOWN:
			reactor.update_selected(-1);
			break;
		case ' ':
			reactor.toggle_selected();
			break;
		case 't':
			speed *= 10;
			break;
		case 'g':
			speed /= 10;
			break;
		case 'r':
			valve.open(0.001);
			break;
		case 'f':
			valve.open(-0.001);
			break;
		case 'e':
			pump.change_speed(0.01);
			break;
		case 'd':
			pump.change_speed(-0.01);
			break;
		}

		if(do_clock)
		{
			long now = get_now();

			while(clock + 1e6 / framerate > now)
			{
				usleep(clock + 1e6 / framerate - now);
				now = get_now();
			}

			clock += 1e6 / framerate;
		}
	}

	return 0;
}

