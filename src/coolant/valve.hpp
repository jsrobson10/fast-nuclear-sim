
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class valve
{
	const double max;

	fluid_holder* const src;
	fluid_holder* const dst;

	double state = 0;

public:

	valve(fluid_holder& src, fluid_holder& dst, double max);

	void open(double v);
	void update(double secs);
	void set_state(double v);
	
	constexpr double get_state() const { return state; }
};

};

