
#include "builder.hpp"

using namespace sim::reactor;

sim::reactor::reactor sim::reactor::builder(const int W, const int H, fuel::fuel_rod fr, control::control_rod cr, coolant::pipe p, const char** lines)
{
	std::unique_ptr<rod> arr[W * H];
	
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
		case 'G':
			r = new control::graphite_rod();
			break;
		case 'H':
			r = new coolant::heater();
			break;
		case ' ':
			r = new coolant::pipe(p);
			break;
		case '#':
			r = new rod();
			break;
		}

		arr[y * W + x] = std::unique_ptr<rod>(std::move(r));
	}

	return reactor(arr, W, H);
}

