
#include "vessel.hpp"
#include "../../constants.hpp"
#include "../../conversions/temperature.hpp"

#include <cmath>

using namespace sim::reactor::coolant;

vessel::vessel(double level, double volume, sim::coolant::fluid_t fluid) : volume(volume), fluid(fluid)
{
	this->level = level;
}

void vessel::update()
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

double vessel::add_heat(double t1)
{
	double t2 = get_heat();
	double t = t1 - t2;
	double m1 = 1000000;
	double m2 = (fluid.l_to_g(level) + steam) * fluid.jPgk;
	double m = m1 + m2;
	
   	return heat = t1 - t * m2 / m;
}

double vessel::extract_steam(double dt, double a, double p2)
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

double vessel::get_pressure() const
{
	double T = conversions::temperature::c_to_k(heat);
	double V = (volume - level) * 0.001;
	double n = fluid.g_to_mol(steam);
	
	return (n * T * constants::R) / V;
}

std::ostream& operator<<(std::ostream& o, const vessel& v)
{
	o << "Volume: " << v.get_volume() << " L\n";
	o << "Level: " << v.get_level() << " L\n";
	o << "Steam: " << v.get_steam() << " g\n";
	o << "Heat: " << v.get_heat() << " C\n";
	o << "Pressure: " << (v.get_pressure() * 0.001) << " kPa\n";

	return o;
}


