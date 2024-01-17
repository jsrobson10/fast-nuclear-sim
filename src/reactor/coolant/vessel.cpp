
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

	steam += n;
	level -= fluid.g_to_l(n);

	if(fluid.g_to_l(steam) > volume)
	{
		steam = fluid.l_to_g(volume);
		level = 0;
	}
}

double vessel::add_heat(double t1)
{
	double t2 = get_heat();
	double t = t1 - t2;
	double m1 = 1000;
	double m2 = level + fluid.g_to_l(steam);
	double m = m1 + m2;
	
   	return heat = t1 - t * m2 / m;
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


