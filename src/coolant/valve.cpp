
#include "valve.hpp"

using namespace sim::coolant;

valve::valve(fluid_holder& src, fluid_holder& dst, double max) : src(&src), dst(&dst), max(max)
{

}

void valve::set_state(double v)
{
	if(v > 1) v = 1;
	if(v < 0) v = 0;
	state = v;
}

void valve::open(double v)
{
	set_state(state + v);
}

void valve::update(double dt)
{
//	rate = a->extract_steam(dt, state * max, pressure) / dt; TODO
}

