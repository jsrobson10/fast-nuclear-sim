
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class pump
{
	fluid_holder* const src;
	fluid_holder* const dst;

	const double mass; // grams
	const double radius; // meters
	const double l_per_rev; // litres
	const double friction; // J/rev

	double velocity = 0; // m/s
	double power = 0; // W

public:

	bool powered = true;
	bool idling = false;

	pump(fluid_holder* src, fluid_holder* dst, double mass, double radius, double l_per_rev, double friction);

	double get_flow() const; // g/s
	double get_rpm() const; // rev/min

	const char* get_state_string();
	void update(double dt);
};

};

