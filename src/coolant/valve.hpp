
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class valve
{
	const double max;

	fluid_holder* const src;
	fluid_holder* const dst;

	double open_speed = 0;
	double state = 0;

public:

	valve(fluid_holder* src, fluid_holder* dst, double max);

	void update(double secs);
	void add_open_speed(double v);
	void clear_open_speed();
	
	constexpr double get_state() const { return state; }
};

};

