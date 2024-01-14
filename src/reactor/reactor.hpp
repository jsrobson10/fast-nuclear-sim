
#pragma once

#include "rod.hpp"

#include <array>

namespace sim::reactor
{

template <int W, int H>
struct reactor
{
	const static int width = W;
	const static int height = H;
	
	std::array<std::array<rod*, H>, W> rods;

	int cursor_x = 0;
	int cursor_y = 0;

	reactor(std::array<rod*, W * H> rods)
	{
		for(int y = 0; y < H; y++)
		for(int x = 0; x < W; x++)
		{
			this->rods[x][y] = rods[y * W + x];
		}
	}

	void update(double secs)
	{
		// do interactions
		for(int x = 1; x < W; x++)
		{
			rods[x][0]->interact(rods[x - 1][0], secs);
		}

		for(int y = 1; y < H; y++)
		{
			rods[0][y]->interact(rods[0][y - 1], secs);
		}

		for(int y = 1; y < H; y++)
		for(int x = 1; x < W; x++)
		{
			rod* r = rods[x][y];
			r->interact(rods[x - 1][y], secs);
			r->interact(rods[x][y - 1], secs);
		}

		// do updates
		for(int y = 0; y < H; y++)
		for(int x = 0; x < W; x++)
		{
			rods[x][y]->update(secs);
		}
	}

	void update_selected(double v)
	{
		for(int y = 0; y < H; y++)
		for(int x = 0; x < W; x++)
		if(rods[x][y]->is_selected())
		{
			rods[x][y]->update_selected(v);
		}
	}

	void move_cursor(int d)
	{
		for(;;)
		{
			cursor_x += d;

			while(cursor_x >= W)
			{
				cursor_x -= W;
				cursor_y += 1;
			}

			while(cursor_x < 0)
			{
				cursor_x += W;
				cursor_y -= 1;
			}

			cursor_y %= H;

			if(cursor_y < 0)
			{
				cursor_y += H;
			}

			if(rods[cursor_x][cursor_y]->should_select())
			{
				return;
			}
		}
	}

	void toggle_selected()
	{
		if(rods[cursor_x][cursor_y]->should_select())
		{
			rods[cursor_x][cursor_y]->toggle_selected();
		}
	}
};

}

