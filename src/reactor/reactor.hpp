
#pragma once

#include "rod.hpp"

#include <array>
#include <random>
#include <algorithm>
#include <iostream>

namespace sim::reactor
{

template <int W, int H>
struct reactor
{
	constexpr const static int width = W;
	constexpr const static int height = H;
	constexpr const static int size = W*H;
	
	rod* rods[size];

	int cursor = 0;

	reactor(std::array<rod*, size> rods)
	{
		for(int i = 0; i < size; i++)
		{
			this->rods[i] = rods[i];
		}
	}

	void update(std::mt19937& rand, double secs)
	{
		int rods_lookup[size];

		for(int i = 0; i < size; i++)
		{
			rods_lookup[i] = i;
		}

		for(int i = 0; i < size; i++)
		{
			rods[i]->update(secs);
		}

		update_interactions(rand, rods_lookup, secs / 2);
	}

	void update_selected(int v)
	{
		for(int i = 0; i < size; i++)
		{
			rod* r = rods[i];

			if(r->is_selected())
			{
				r->update_rod_selected(v);
			}
		}
	}

	int move_cursor(int d)
	{
		for(int i = 0; i < size; i++)
		{
			cursor = (cursor + d) % size;
			
			if(cursor < 0)
			{
				cursor += size;
			}

			if(rods[cursor]->should_select())
			{
				return cursor;
			}
		}

		return 0;
	}

	void toggle_selected()
	{
		if(rods[cursor]->should_select())
		{
			rods[cursor]->toggle_selected();
		}
	}

private:

	void update_tile(std::mt19937& rand, double secs, int i, int x, int y)
	{
		std::array<int, 2> nb_lookup[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
		std::shuffle(nb_lookup, &nb_lookup[3], rand);

		for(int j = 0; j < 4; j++)
		{
			int xp = x + nb_lookup[j][0];
			int yp = y + nb_lookup[j][1];
			
			if(xp >= 0 && yp >= 0 && xp < width && yp < height)
			{
				rods[i]->interact(rods[yp * width + xp], secs / 2);
			}
		}
	}

	void update_interactions(std::mt19937& rand, int* rods_lookup, double secs)
	{
		std::shuffle(rods_lookup, &rods_lookup[size - 1], rand);

		for(int id = 0; id < size; id++)
		{
			int i = rods_lookup[id];
			int x = i % width;
			int y = i / width;

			for(int j = 0; j < 4; j++)
			{
				update_tile(rand, secs, i, x, y);
			}
		}
	}
};

}

