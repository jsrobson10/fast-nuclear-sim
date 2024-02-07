
#include "fluid_holder.hpp"
#include "../util/constants.hpp"
#include "../conversions/temperature.hpp"
#include "../reactor/fuel/half_life.hpp"

#include <cmath>
#include <iostream>

using namespace sim::coolant;

fluid_holder::fluid_holder(fluid_t fluid, double volume, double extra_mass) : fluid(fluid), volume(volume), extra_mass(extra_mass)
{

}

double fluid_holder::add_heat(double m1, double t1)
{
	double t2 = get_heat();
	double t = t1 - t2;
	double m2 = get_thermal_mass();
	double m = m1 + m2;

	if(m1 == 0 || m2 == 0)
		return t1;
	
   	heat = t1 - t * m2 / m;

	return heat;
}

double fluid_holder::add_fluid(double v2, double t2)
{
	if(level + v2 > volume - 1e-3)
	{
		v2 = volume - level - 1e-3;
	}

	double m1 = get_thermal_mass();
	double m2 = fluid.l_to_g(v2);
	
	double t1 = get_heat();
	double t = t1 - t2;

	heat = t1 - t * m2 / (m1 + m2);
	level += v2;

	return v2;
}

double fluid_holder::extract_fluid(double amount)
{
	if(amount < level - 1e-3)
	{
		level -= amount - 1e-3;
	}

	else
	{
		amount = level;
		level = 0;
	}
	
	return amount;
}

void fluid_holder::add_steam(double m2, double t2)
{
	double m1 = get_thermal_mass();
	double t1 = heat;
	double m = m1 + m2;

	if(m > 0)
	{
		heat = t1 - (t1 - t2) * m2 / (m1 + m2);
	}

	steam += m2;
}

double fluid_holder::calc_pressure(double heat, double volume, double mol)
{
	double T = conversions::temperature::c_to_k(heat);
	double V = volume * 0.001;

	return V == 0 ? 0 : (mol * T * constants::R) / V;
}

double fluid_holder::calc_pressure_mol(double heat, double volume, double pressure)
{
	double T = conversions::temperature::c_to_k(heat);
	double V = volume * 0.001;

	return (V * pressure) / (T * constants::R);
}

double fluid_holder::get_pressure() const
{
	return calc_pressure(heat, get_steam_volume(), fluid.g_to_mol(get_steam()));
}

double fluid_holder::get_steam_density() const
{
	double v = get_steam_volume();
	return v > 0 ? steam / v : 0;
}

void fluid_holder::update(double secs)
{
	double mass = get_thermal_mass();

	if(mass > 0)
	{
		double V = get_steam_volume() * 0.001;
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

		if(l > v)
		{
			l = v;
			s = 0;
		}

		double diff = s - steam;

		steam = s;
		level = fluid.g_to_l(l);
		heat -= diff * fluid.jPg / mass;
	}
}

