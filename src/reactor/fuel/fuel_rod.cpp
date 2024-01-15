
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
	switch(type)
	{
	case val_t::HEAT:
		return 1.0 / 16.0;
	case val_t::N_SLOW:
		return 1.0 / 4.0;
	case val_t::N_FAST:
		return 1.0 / 2.0;
	}

	return 0;
}

void fuel_rod::update(double secs)
{
	update_rod(secs);
	
	s.add_slow_neutrons(vals[val_t::N_SLOW]);

	vals[val_t::HEAT] += s.extract_energy();
	vals[val_t::N_FAST] += s.extract_fast_neutrons();
	vals[val_t::N_SLOW] = 0;
	
	s.update(secs);
}

