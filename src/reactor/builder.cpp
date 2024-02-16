
#include "builder.hpp"
#include "control/boron_rod.hpp"
#include "control/graphite_rod.hpp"
#include "coolant/pipe.hpp"
#include "coolant/heater.hpp"

#include <cmath>
#include <vector>
#include <cstdlib>

using namespace Sim::Reactor;

Sim::Reactor::Reactor Sim::Reactor::Builder(const int W, const int H, const double CW, const double CH, Fuel::FuelRod fr, Coolant::Vessel* v, const char** lines)
{
	std::vector<std::unique_ptr<Rod>> arr(W * H);
	
	for(int y = 0; y < H; y++)
	for(int x = 0; x < W; x++)
	{
		char c = lines[y][x];
		std::unique_ptr<Rod> r;

		switch(c)
		{
		case 'F':
			r = std::make_unique<Fuel::FuelRod>(fr);
			break;
		case 'C':
			r = std::make_unique<Control::BoronRod>(v);
			break;
		case 'G':
			r = std::make_unique<Control::GraphiteRod>();
			break;
		case 'H':
			r = std::make_unique<Coolant::Heater>();
			break;
		case 'P':
			r = std::make_unique<Coolant::Pipe>(v);
			break;
		default:
			r = std::make_unique<Rod>();
			break;
		}

		arr[y * W + x] = std::move(r);
	}

	return Reactor(&arr[0], W, H, CW, CH);
}

std::unique_ptr<Rod> Sim::Reactor::load_rod(const Json::Value& node, Coolant::Vessel* v)
{
	int id = node["id"].asInt();

	switch(id)
	{
	case 1:
		return std::make_unique<Fuel::FuelRod>(node);
	case 2:
		return std::make_unique<Coolant::Pipe>(node, v);
	case 3:
		return std::make_unique<Coolant::Heater>(node);
	case 4:
		return std::make_unique<Control::GraphiteRod>(node);
	case 5:
		return std::make_unique<Control::BoronRod>(node, v);
	}

	return std::make_unique<Rod>();
}

