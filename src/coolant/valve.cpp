
#include "valve.hpp"

using namespace sim::coolant;

valve::valve(fluid_holder* src, fluid_holder* dst, double max) : src(src), dst(dst), max(max)
{

}

void valve::add_open_speed(double v)
{
	open_speed += v;
}

void valve::clear_open_speed()
{
	open_speed = 0;
}

void valve::update(double dt)
{
	state += open_speed * dt;
	if(state > 1) state = 1;
	if(state < 0) state = 0;
}

