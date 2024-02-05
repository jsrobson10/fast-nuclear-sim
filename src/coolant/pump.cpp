
#include "pump.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

pump::pump(fluid_holder* src, fluid_holder* dst, double mass, double radius, double power, double l_per_rev, double friction) :
		src(src), dst(dst), mass(mass), radius(radius), l_per_rev(l_per_rev), friction(friction)
{
	this->power = power;
}

double pump::get_flow() const
{
	return l_per_rev * get_rpm() * 60;
}

double pump::get_flow_mass() const
{
	return src->fluid.l_to_g(get_flow());
}

double pump::get_rpm() const
{
	return velocity / (M_PI * mass * 0.001 * radius * radius);
}

const char* pump::get_state_string()
{
	if(!powered)
	{
		return "Off";
	}

	if(idling && std::abs(get_flow()) < 1e-3)
	{
		return "Idle";
	}

	if(idling)
	{
		return "Coasting";
	}

	return "Revving";
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
	idling = false;

	if(powered && !idling)
	{
		velocity += calc_work(dt * power, mass);
	}
	
	fluid_holder fh_src(*src);
	fluid_holder fh_dst(*dst);

	double src_heat = src->get_heat();
	double p_diff_1 = dst->get_pressure() - src->get_pressure();
	double src_volume = fh_src.extract_fluid(get_flow() * dt);
	double dst_volume = fh_dst.add_fluid(src_volume, src_heat);

	src->extract_fluid(dst_volume);
	dst->add_fluid(dst_volume, src_heat);
	
	double p_diff_2 = dst->get_pressure() - src->get_pressure();
	double p_diff = (p_diff_1 + p_diff_2) / 2;
	double work = p_diff * dst_volume * 0.001 + get_rpm() * 60 * dt * friction;

	velocity -= calc_work(work, mass);

	if(dst->get_level() > 400 || src->get_level() < 10)
	{
//		idling = true;
	}

	else
	{
//		idling = false;
	}
}

