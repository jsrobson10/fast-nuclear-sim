
#include "pump.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

pump::pump(fluid_holder* src, fluid_holder* dst, double mass, double radius, double l_per_rev, double friction) :
		src(src), dst(dst), mass(mass), radius(radius), l_per_rev(l_per_rev), friction(friction)
{
	power = 1e3;
}

double pump::get_flow() const
{
	return l_per_rev * get_rpm() / 60;
}

double pump::get_rpm() const
{
	return velocity / (M_PI * mass * 0.001 * radius * radius);
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
	velocity += calc_work(dt * power, mass);
	
	double src_heat = src->get_heat();
	double p_diff_1 = dst->get_pressure() - src->get_pressure();
	double src_volume = src->extract_fluid(get_flow() * dt);
	double dst_volume = dst->add_fluid(src_volume, src_heat);

	if(dst_volume < src_volume)
	{
		src->add_fluid(src_volume - dst_volume, src_heat);
	}
	
	double p_diff_2 = dst->get_pressure() - src->get_pressure();
	double p_diff = (p_diff_1 + p_diff_2) / 2;
	double work = p_diff * dst_volume + get_rpm() * 60 * dt * friction;

	velocity -= calc_work(work, mass);

	if(dst->get_level() > 400 || src->get_level() < 50)
	{
		power = 0;
	}

	else
	{
		power = 1e3;
	}

	std::cout << "RPM: " << get_rpm() << "\t";
	std::cout << "Flow: " << get_flow() << std::endl;
	std::cout << "Work Done: " << work << " J\n";
	std::cout << "Src Volume: " << src_volume << "\n";
}

