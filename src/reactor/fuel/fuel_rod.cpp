
#include "fuel_rod.hpp"

using namespace sim::reactor::fuel;

fuel_rod::fuel_rod(double fuel, double mass) : s(fuel, mass)
{

}

void fuel_rod::display(std::ostream& o) const
{
	o << "Fuel: " << s.get_fuel() << " / " << s.get_mass() << " mol\n";
	o << "Efficiency: " << (s.get_efficiency() * 100) << " %\n";
	o << "Output: " << s.get_energy() << " W/s\n";
	o << "Iodine: " << s.get_i_135() << " mol\n";
	o << "Xenon: " << s.get_xe_135() << " mol\n";
}

void fuel_rod::update(double secs)
{
	update_rod(secs);
	
	s.clear_energy();
	s.clear_fast_neutrons();
	s.add_slow_neutrons(vals[val_t::N_SLOW]);
	s.update(secs);

	vals[val_t::HEAT] += s.get_energy() * secs;
	vals[val_t::N_FAST] += s.get_fast_neutrons();
	vals[val_t::N_SLOW] = 0;
}

