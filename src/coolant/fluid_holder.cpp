
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

double fluid_holder::calc_pressure_vol(double heat, double pressure, double mol)
{
	double T = conversions::temperature::c_to_k(heat);
	
	return 1000 * (mol * T * constants::R) / pressure;
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
		// use ideal gas law to get target steam pressure
		double heat_k = conversions::temperature::c_to_k(heat);
		double target_pressure = fluid.vapor_pressure.calc_p(heat);

		double K = heat_k;
		double R = 1000 * constants::R;
		double P = target_pressure;
		double J_m = fluid.jPg * fluid.gPmol;
		double L_m = fluid.gPmol / fluid.gPl;
		double n_g = fluid.g_to_mol(steam);
		double n_l = fluid.l_to_mol(level);
		double V_t = volume;

		double n = (-K*R*n_g - L_m*P*n_l + P*V_t)/(K*R - L_m*P) * 0.5;

		/*if(std::abs(n.imag()) > std::numeric_limits<double>::epsilon())
		{
			throw std::runtime_error("Nonzero imaginary component");
		}*/

		double l = level - fluid.mol_to_l(n);

		if(l < 0)
		{
			n -= fluid.l_to_mol(l);
			l = 0;
		}

		level = l;
		steam += fluid.mol_to_g(n);
		heat -= n * J_m / mass;
	}
}

