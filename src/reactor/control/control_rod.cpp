
#include "control_rod.hpp"

#include <cmath>

using namespace sim::reactor::control;

control_rod::control_rod(coolant::vessel& v, double limit, double max) : coolant::pipe(v), limit(limit), max(max)
{
	
}

void control_rod::display(std::ostream& o) const
{
	o << "Inserted: " << (inserted * 100) << "%\n";
	o << "Use: " << absorbed << " / " << limit << " mol\n";
};

void control_rod::set_reactivity(double a)
{
	inserted = 1 - a;
}

void control_rod::update(double secs)
{
	update_rod(secs);

	double k = (1 - absorbed / limit) * inserted * max;
	double m = 1 - std::pow(0.5, secs * -std::log2(1 - k));
	double r_fast = vals[val_t::N_FAST] * m;
	double r_slow = vals[val_t::N_SLOW] * m;

	vals[val_t::N_FAST] -= r_fast;
	vals[val_t::N_SLOW] -= r_slow;
	absorbed += r_fast + r_slow;
	
	update_pipe(secs);
}

void control_rod::update_selected(double a)
{
	inserted += a;

	if(inserted > 1) inserted = 1;
	if(inserted < 0) inserted = 0;
}

