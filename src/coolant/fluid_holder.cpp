
#include "fluid_holder.hpp"
#include "../constants.hpp"
#include "../conversions/temperature.hpp"
#include "../reactor/fuel/half_life.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

fluid_holder::fluid_holder(fluid_t fluid, double volume) : fluid(fluid), volume(volume)
{

}

double fluid_holder::add_heat(double m1, double t1)
{
	double t2 = get_heat();
	double t = t1 - t2;
	double m2 = (fluid.l_to_g(level) + steam) * fluid.jPgk;
	double m = m1 + m2;

	if(m1 == 0 || m2 == 0)
		return t1;
	
   	heat = t1 - t * m2 / m;

	return heat;
}

double fluid_holder::add_fluid(double v2, double t2)
{
	double v1 = level;
	double t1 = get_heat();
	double t = t1 - t2;

	if(level + v2 > volume)
	{
		v2 = volume - level;
	}

	heat = t1 - t * v2 / (v1 + v2);
	level += v2;

	return v2;
}

double fluid_holder::extract_fluid(double amount)
{
	if(amount < level)
	{
		level -= amount;
	}

	else
	{
		amount = level;
		level = 0;
	}
	
	return amount;
}

double fluid_holder::extract_steam(double dt, double a, double p2)
{
	// calculate the mass moved
	double p1 = get_pressure();
	double p = (p1 - p2) * 0.001; // mPa or g/m/s^2

	if(p == 0)
	{
		return 0;
	}

	double V = (volume - level) * 0.001; // m^3
	double mass = std::min(dt * a * p / std::sqrt( V * std::abs(p) / steam ), steam);

	if(std::isnan(mass))
	{
		return 0;
	}

	steam -= mass;
	return mass;
}

double fluid_holder::get_pressure() const
{
	double T = conversions::temperature::c_to_k(heat);
	double V = (volume - level) * 0.001;
	double n = fluid.g_to_mol(steam);
	
	return (n * T * constants::R) / V;
}

void fluid_holder::update(double secs)
{
	double V = (volume - level) * 0.001;
	double P = fluid.vapor_pressure.calc_p(heat);
	double T = conversions::temperature::c_to_k(heat);
	double n = fluid.mol_to_g((V * P) / (T * constants::R)) - steam;

	double s = steam + n;
	double l = fluid.l_to_g(level) - n;
	double v = fluid.l_to_g(volume);

	if(l < 0)
	{
		s += l;
		l = 0;
	}

	if(s > v)
	{
		s = v;
		l = 0;
	}

	if(l > v)
	{
		l = v;
		s = 0;
	}

	double diff = s - steam;

	steam = s;
	level = fluid.g_to_l(l);
	heat -= diff * fluid.jPg / (fluid.l_to_g(level) + steam) / fluid.jPgk;

}

