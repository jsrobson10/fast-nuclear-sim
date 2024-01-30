
#include "builder.hpp"

#include <cmath>
#include <vector>
#include <cstdlib>

using namespace sim::reactor;

sim::reactor::reactor sim::reactor::builder(const int W, const int H, const double CW, const double CH, fuel::fuel_rod fr, control::boron_rod br, coolant::vessel& v, const char** lines)
{
	std::vector<std::unique_ptr<rod>> arr(W * H);
	
	for(int y = 0; y < H; y++)
	for(int x = 0; x < W; x++)
	{
		char c = lines[y][x];
		std::unique_ptr<rod> r;

		switch(c)
		{
		case 'F':
			r = fr.clone();
			break;
		case 'C':
			r = br.clone();
			break;
		case 'G':
			r = std::make_unique<control::graphite_rod>();
			break;
		case 'H':
			r = std::make_unique<coolant::heater>();
			break;
		case 'P':
			r = std::make_unique<coolant::pipe>(v);
			break;
		case ' ':
			r = std::make_unique<rod>();
			break;
		}

		arr[y * W + x] = std::move(r);
	}

	return reactor(&arr[0], W, H, CW, CH);
}

