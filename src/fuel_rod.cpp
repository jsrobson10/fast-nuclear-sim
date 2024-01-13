
#include "fuel_rod.hpp"

using namespace sim;

fuel_rod::fuel_rod(double f, double m) : fuel(f, m)
{

}

void fuel_rod::update(double secs)
{
	double n = fuel.extract_fast_neutrons();
	double n_slow = n * 0.5 * reactivity;
	fuel.add_slow_neutrons(n_slow);
	neutrons_absorbed += n * 0.5 * (1 - reactivity);
	neutrons_free += n_slow;
	
	fuel.update(secs);
	

	temperature += fuel.extract_energy() / fuel.get_mass();
}

void fuel_rod::add_neutrons(double amount)
{
	fuel.add_slow_neutrons(amount);
}

double fuel_rod::extract_free_neutrons()
{
	double v = neutrons_free;
	neutrons_free = 0;
	return v;
}

double fuel_rod::extract_heat(double k, double o)
{
	double v = k * (temperature - o);
	temperature -= v;
	return v;
}

void fuel_rod::add_heat(double amount)
{
	temperature += amount;
}

void fuel_rod::set_reactivity(double v)
{
	reactivity = v;
}

void fuel_rod::display(std::ostream& o) const
{
	o << "Temperature: " << temperature << "\n";
	o << "Reactivity: " << reactivity << "\n";
	o << "Fuel: " << fuel.get_fuel() << " / " << fuel.get_mass() << "\n";
	o << "Neutrons:\n  Absorbed: " << neutrons_absorbed << "\n  Free: " << neutrons_free << "\n";
}

