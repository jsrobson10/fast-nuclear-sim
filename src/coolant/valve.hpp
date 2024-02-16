
#pragma once

#include "fluid_holder.hpp"
#include "../util/pid.hpp"

namespace Sim::Coolant
{

class Valve
{
	const double max;

	FluidHolder* const src;
	FluidHolder* const dst;

	double speed = 0;
	double state = 0;
	double flow = 0; // L/s

	bool auto_on = false;
	double auto_th = 0; // C

	Util::PID pid {1e-3, -1e-3, 100, 0, 0};

public:

	Valve(FluidHolder* src, FluidHolder* dst, double state, double max);
	Valve(const Json::Value& node, FluidHolder* src, FluidHolder* dst);

	void update(double secs);
	void add_open_speed(double v);
	void clear_open_speed();
	void set_auto(bool state);
	void toggle_auto();
	
	operator Json::Value() const;
	
	constexpr double get_state() const { return state; }
	constexpr double get_flow() const { return flow; }
	constexpr double get_setpoint() const { return auto_th; }
	constexpr bool get_auto() const { return auto_on; }
};

};

