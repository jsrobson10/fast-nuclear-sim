
#include "builder.hpp"

#include <cmath>
#include <vector>

using namespace sim::reactor;

sim::reactor::reactor sim::reactor::builder(const int W, const int H, const double CW, const double CH, fuel::fuel_rod fr, control::boron_rod br, coolant::vessel& v, const char** lines)
{
	std::vector<std::unique_ptr<rod>> arr(W * H);
	
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
			r = new control::boron_rod(br);
			break;
		case 'G':
			r = new control::graphite_rod();
			break;
		case 'H':
			r = new coolant::heater();
			break;
		case 'P':
			r = new coolant::pipe(v);
			break;
		case ' ':
			r = new rod();
			break;
		}

		arr[y * W + x] = std::unique_ptr<rod>(std::move(r));
	}

	return reactor(&arr[0], W, H, CW, CH);
}

