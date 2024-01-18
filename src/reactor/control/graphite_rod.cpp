
#include "graphite_rod.hpp"

#include <cmath>

using namespace sim::reactor::control;

void graphite_rod::display(std::ostream& o) const
{
	o << "Inserted: " << (inserted * 100) << "%\n";
};

double graphite_rod::get_k(val_t type) const
{
	if(type == val_t::HEAT) return 0.5;
	
	return inserted * 0.5;
}

void graphite_rod::set_reactivity(double a)
{
	inserted = a;
}

void graphite_rod::update(double secs)
{
	update_rod(secs);
	
	vals[val_t::N_SLOW] = vals[val_t::N_FAST];
	vals[val_t::N_FAST] = 0;
}

void graphite_rod::update_selected(double a)
{
	inserted += a;

	if(inserted > 1) inserted = 1;
	if(inserted < 0) inserted = 0;
}

