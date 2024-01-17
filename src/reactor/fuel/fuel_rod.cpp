
#include "fuel_rod.hpp"

using namespace sim::reactor::fuel;

fuel_rod::fuel_rod(double fuel, double mass) : s(fuel, mass)
{

}

void fuel_rod::display(std::ostream& o) const
{
	o << "Fuel: " << s.get_fuel() << " / " << s.get_mass() << "\n";
	o << "Efficiency: " << s.get_efficiency() << "\n";
	o << "Energy: +" << s.get_energy() << "\n";
}

double fuel_rod::get_k(val_t type) const
{
	return 0.5;
}

void fuel_rod::update(double secs)
{
	update_rod(secs);
	
	s.add_slow_neutrons(vals[val_t::N_SLOW]);

	vals_in[val_t::HEAT] += s.extract_energy();
	vals_in[val_t::N_FAST] += s.extract_fast_neutrons();
	vals_in[val_t::N_SLOW] -= vals[val_t::N_SLOW];
	
	s.update(secs);
}

