
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
			rods[x][0]->interact(rods[x - 1][0]);
		}

		for(int y = 1; y < H; y++)
		{
			rods[0][y]->interact(rods[0][y - 1]);
		}

		for(int y = 1; y < H; y++)
		for(int x = 1; x < W; x++)
		{
			rod* r = rods[x][y];
			r->interact(rods[x - 1][y]);
			r->interact(rods[x][y - 1]);
		}

		// do updates
		for(int y = 0; y < H; y++)
		for(int x = 0; x < W; x++)
		{
			rods[x][y]->update(secs);
		}
	}
};

}

