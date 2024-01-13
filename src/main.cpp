
#include "reactor.hpp"

#include <sstream>
#include <unistd.h>
#include <curses.h>

void draw_text(int x, int y, const char* str)
{
	for(int i = 0;; i++)
	{
		const char* start = str;
		char c = (str++)[0];

		while(c != '\n' && c != '\0')
		{
			c = (str++)[0];
		}

		mvaddnstr(x + i, y, start, (size_t)(str - start));

		if(c == '\0') return;
	}
}

void draw_box(int x, int y, int h, int w)
{
	mvaddch(x, y, '+');

	for(int i = 0; i < w - 2; i++)
	{
		addch('-');
	}

	addch('+');

	for(int i = 0; i < h - 2; i++)
	{
		mvaddch(x + i + 1, y, '|');
		mvaddch(x + i + 1, y + w - 1, '|');
	}
	
	mvaddch(x + h - 1, y, '+');

	for(int i = 0; i < w - 2; i++)
	{
		addch('-');
	}

	addch('+');
}

int main()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	
	sim::reactor reactor(5, {100, 200});

	for(;;)
	{
		for(int i = 0; i < 1e3; i++)
		{
			reactor.update(1e-3);
		}

		erase();
		draw_text(1, 0, "Reactor Core:");

		const int X = 3, Y = 4;
		const int W = 32, H = 8;

		for(int x = 0; x < reactor.get_size(); x++)
		for(int y = 0; y < reactor.get_size(); y++)
		{
			std::stringstream ss;
			reactor.display(ss, x, y);
			
			int px = X + (H - 1) * y;
			int py = Y + (W - 1) * x;

			draw_text(px + 1, py + 2, ss.str().c_str());
			draw_box(px, py, H, W);
		}

		refresh();
	}

	return 0;
}

