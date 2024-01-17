
#include "control_rod.hpp"

#include <cmath>

using namespace sim::reactor::control;

control_rod::control_rod(double limit, double max)
{
	this->limit = limit;
	this->max = max;
}

void control_rod::display(std::ostream& o) const
{
	o << "Inserted: " << (inserted * 100) << "%\n";
	o << "Absorbed: " << absorbed << " / " << limit << "\n";
};

double control_rod::get_k(val_t type) const
{
	return 0.5;
}

void control_rod::set_reactivity(double a)
{
	inserted = 1 - a;
}

void control_rod::update(double secs)
{
	update_rod(secs);
	
	double m = 1 - std::pow(0.5, (1 - absorbed / limit) * inserted * max);
	double r_fast = vals[val_t::N_FAST] * m;
	double r_slow = vals[val_t::N_SLOW] * m;

	vals[val_t::N_FAST] -= r_fast;
	vals[val_t::N_SLOW] -= r_slow;
	absorbed += r_fast + r_slow;
}

void control_rod::update_selected(double a)
{
	inserted += a;

	if(inserted > 1) inserted = 1;
	if(inserted < 0) inserted = 0;
}

