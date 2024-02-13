
#include "builder.hpp"
#include "control/boron_rod.hpp"
#include "control/graphite_rod.hpp"
#include "coolant/pipe.hpp"
#include "coolant/heater.hpp"

#include <cmath>
#include <vector>
#include <cstdlib>

using namespace sim::reactor;

sim::reactor::reactor sim::reactor::builder(const int W, const int H, const double CW, const double CH, fuel::fuel_rod fr, coolant::vessel* v, const char** lines)
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
			r = std::make_unique<fuel::fuel_rod>(fr);
			break;
		case 'C':
			r = std::make_unique<control::boron_rod>(v);
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
		default:
			r = std::make_unique<rod>();
			break;
		}

		arr[y * W + x] = std::move(r);
	}

	return reactor(&arr[0], W, H, CW, CH);
}

std::unique_ptr<rod> sim::reactor::load_rod(const Json::Value& node, coolant::vessel* v)
{
	int id = node["id"].asInt();

	switch(id)
	{
	case 1:
		return std::make_unique<fuel::fuel_rod>(node);
	case 2:
		return std::make_unique<coolant::pipe>(node, v);
	case 3:
		return std::make_unique<coolant::heater>(node);
	case 4:
		return std::make_unique<control::graphite_rod>(node);
	case 5:
		return std::make_unique<control::boron_rod>(node, v);
	}

	return std::make_unique<rod>();
}

