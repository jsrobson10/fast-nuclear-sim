
#include "pump.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

pump::pump(fluid_holder* src, fluid_holder* dst, double mass, double radius, double power, double l_per_rev, double friction) :
		src(src),
		dst(dst),
		mass(mass),
		radius(radius),
		l_per_rev(l_per_rev),
		friction(friction),
		max_power(power)
{
	
}

double pump::get_flow_target() const
{
	return l_per_rev * get_rpm() * 60;
}

double pump::get_flow() const
{
	return flow;
}

double pump::get_flow_mass() const
{
	return src->fluid.l_to_g(flow);
}

double pump::get_rpm() const
{
	return velocity / (M_PI * mass * 0.001 * radius * radius);
}

double pump::get_power() const
{
	return power;
}

const char* pump::get_state_string()
{
	if(!powered) return "Off";
	if(power == 0) return "Idle";
	return "On";
}

static double calc_work(double j, double mass)
{
	double m = 1;

	if(j < 0)
	{
		m = -1;
	}

	return m * std::sqrt(m * j / (mass * 0.001));
}

void pump::update(double dt)
{
	if(powered)
	{
		power = pid.calculate(dt, src->get_volume() / 2, src->get_steam_volume());
		velocity += calc_work(dt * power * max_power, mass);
	}
	
	fluid_holder fh_src(*src);
	fluid_holder fh_dst(*dst);

	double src_heat = src->get_heat();
	double p_diff_1 = dst->get_pressure() - src->get_pressure();
	double src_volume = fh_src.extract_fluid(get_flow_target() * dt);
	double dst_volume = fh_dst.add_fluid(src_volume, src_heat);

	src->extract_fluid(dst_volume);
	dst->add_fluid(dst_volume, src_heat);
	
	double p_diff_2 = dst->get_pressure() - src->get_pressure();
	double p_diff = (p_diff_1 + p_diff_2) / 2;
	double work = p_diff * dst_volume * 0.001 + get_rpm() * 60 * dt * friction;

	velocity -= calc_work(work, mass);
	flow = dst_volume / dt;
}

