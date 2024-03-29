
#pragma once

#include "fluid_holder.hpp"
#include "../util/pid.hpp"

namespace Sim::Coolant
{

class Pump
{
	FluidHolder* const src;
	FluidHolder* const dst;

	Util::PID pid {1, 0, 100, 0, 0};

	double flow = 0; // L/s
	double velocity = 0; // m/s
	double power = 0;

public:

	enum mode_t
	{
		SRC,
		DST,
		NONE
	};

	const mode_t mode;
	
	const double mass; // grams
	const double radius; // meters
	const double l_per_rev; // litres
	const double friction; // J/rev
	const double max_power; // W
	const double target; // L
	
	bool powered = false;

	Pump(FluidHolder* src, FluidHolder* dst, double mass, double radius, double power, double l_per_rev, double friction, mode_t mode, double target);
	Pump(const Json::Value& node, FluidHolder* src, FluidHolder* dst);

	double get_flow() const; // L/s
	double get_flow_target() const; // L/s
	double get_flow_mass() const; // g/s
	double get_rpm() const; // rev/min
	double get_power() const; // W
	
	operator Json::Value() const;

	const char* get_state_string();
	void update(double dt);
};

};

