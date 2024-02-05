
#include "fuel_rod.hpp"

#include <cmath>

using namespace sim::reactor::fuel;

constexpr double fuel_density = 19100000; // g/m^3
constexpr double fuel_molar_mass = 238.029; // g/mol
constexpr double fuel_molar_density = fuel_density / fuel_molar_mass; // mol/m^3
constexpr double energy_density = 165e11; // J/mol

fuel_rod::fuel_rod(double fuel) : s(fuel)
{

}

void fuel_rod::display(std::ostream& o) const
{
	double mol = fuel_molar_density * get_volume();
	
	o << "Fuel: " << (s.get_fuel() * mol) << " / " << (s.get_mass() * mol) << " mol\n";
	o << "Efficiency: " << (s.get_efficiency() * 100) << " %\n";
	o << "Output: " << get_energy_output() << " W\n";
	o << "Iodine: " << (s.get_i_135() * mol) << " mol\n";
	o << "Xenon: " << (s.get_xe_135() * mol) << " mol\n";
}

double fuel_rod::get_energy_output() const
{
	double mol = fuel_molar_density * get_volume();

	return s.get_energy() * mol * energy_density;
}

static float map(float v, float imin, float imax, float omin, float omax)
{
	return (v - imin) * (omax - omin) / (imax - imin) + omin;
}

glm::vec4 fuel_rod::get_colour() const
{
	double temp = vals[val_t::HEAT];

	if(temp < 0)
	{
		temp = 0;
	}

	// this should not happen
	if(std::isnan(temp))
	{
		return {1, 0, 1, 1};
	}

	if(temp < 120)
	{
		return {0, map(temp, 0, 120, 0, 1), 1, 1};
	}

	if(temp < 240)
	{
		return {0, 1, map(temp, 120, 240, 1, 0), 1};
	}

	if(temp < 280)
	{
		return {map(temp, 240, 280, 0, 1), 1, 0, 1};
	}

	if(temp < 320)
	{
		return {1, map(temp, 280, 320, 1, 0), 0, 1};
	}

	return {1, 0, 0, 1};
}

void fuel_rod::update(double secs)
{
	update_rod(secs);
	
	double mol = fuel_molar_density * get_volume();
	
	s.clear_energy();
	s.clear_fast_neutrons();
	s.clear_slow_neutrons();
	s.add_slow_neutrons(vals[val_t::N_SLOW] / mol);
	s.update(secs);

	vals[val_t::HEAT] += s.get_energy() * mol * energy_density * secs;
	vals[val_t::N_FAST] += s.get_fast_neutrons() * mol;
	vals[val_t::N_SLOW] = s.get_slow_neutrons() * mol;
}

