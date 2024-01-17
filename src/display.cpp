
#include "display.hpp"

#include <curses.h>
#include <string.h>
#include <stdlib.h>

void display::draw_text(int x, int y, const char* at)
{
	for(int i = 0;; i++)
	{
		const char* start = at;
		char c = (at++)[0];

		while(c != '\n' && c != '\0')
		{
			c = (at++)[0];
		}

		mvaddnstr(x + i, y, start, (size_t)(at - start));

		if(c == '\0')
		{
			return;
		}
	}
}

void display::draw_box(int x, int y, int h, int w)
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
