
#include "fluid_holder.hpp"
#include "../util/constants.hpp"
#include "../conversions/temperature.hpp"
#include "../reactor/fuel/half_life.hpp"

#include <cmath>
#include <iostream>
#include <complex>

using namespace sim::coolant;

typedef std::complex<double> complex;

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
	if(level + v2 <= 0)
	{
		return 0;
	}
	
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
	double V = volume * 0.001;

	return V == 0 ? 0 : (mol * heat * constants::R) / V;
}

double fluid_holder::calc_pressure_mol(double heat, double volume, double pressure)
{
	double V = volume * 0.001;

	return (pressure * V) / (constants::R * heat);
}

double fluid_holder::get_pressure() const
{
	return calc_pressure(conversions::temperature::c_to_k(heat), get_steam_volume(), fluid.g_to_mol(steam));
}

double fluid_holder::get_steam_density() const
{
	double v = get_steam_volume();
	return v > 0 ? steam / v : 0;
}

constexpr double calc_extra_steam(double K, double P, double L_m, double J_m, double n_g, double n_l, double V_t)
{
	double R = sim::constants::R * 1000;
	double n = (P * (V_t - n_l * L_m)) / (R * K) - n_g;

	return n;
}

void fluid_holder::update_base(double secs)
{
	double mass = get_thermal_mass();

	if(mass > 0)
	{
		double K = conversions::temperature::c_to_k(heat);	// K
		double P = fluid.vapor_pressure.calc_p(K);			// Pa
		double R = sim::constants::R;						// J/K/mol

		double J_m = fluid.jPg * fluid.gPmol;	// J/mol
		double n_g = fluid.g_to_mol(steam);		// mol
		double V_g = (volume - level) * 0.001;	// m^3

		double n = (P * V_g) / (R * K) - n_g;	// mol
		double l = level - fluid.mol_to_l(n);	// L

		if(l < 0)
		{
			n -= fluid.l_to_mol(l);
			l = 0;
		}
		
		steam += fluid.mol_to_g(n);

		if(steam < 0)
		{
			l -= fluid.g_to_l(steam);
			n += fluid.g_to_mol(steam);
			steam = 0;
		}
		
		level = l;
		heat -= n * J_m / mass;
	}
}

