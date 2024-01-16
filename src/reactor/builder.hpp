
#pragma once

#include "rod.hpp"
#include "fuel/fuel_rod.hpp"
#include "control/control_rod.hpp"
#include "coolant/pipe.hpp"
#include "coolant/heater.hpp"
#include "reactor.hpp"

namespace sim::reactor
{

template <int W, int H>
reactor<W, H> builder(fuel::fuel_rod fr, control::control_rod cr, coolant::pipe p, std::array<const char*, H> lines)
{
	std::array<rod*, W * H> arr;
	
	for(int y = 0; y < H; y++)
	for(int x = 0; x < W; x++)
	{
		char c = lines[y][x];
		rod* r;

		switch(c)
		{
		case 'F':
			r = new fuel::fuel_rod(fr);
			break;
		case 'C':
			r = new control::control_rod(cr);
			break;
		case 'H':
			r = new coolant::heater();
			break;
		case ' ':
			r = new coolant::pipe(p);
			break;
		case '#':
			r = new rod();
		}

		arr[y * W + x] = r;
	}

	return reactor<W, H>(arr);
}

};

