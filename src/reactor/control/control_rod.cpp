
#include "control_rod.hpp"

using namespace sim::reactor::control;

control_rod::control_rod(double limit)
{
	this->limit = limit;
}

const char* control_rod::get_name() const
{
	return "Control Rod";
}

void control_rod::display(std::ostream& o) const
{
	o << "Inserted: " << (inserted * 100) << "%\n";
	o << "Absorbed: " << absorbed << " / " << limit << "\n";
};

double control_rod::get_k(val_t type) const
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

void control_rod::set_reactivity(double a)
{
	inserted = 1 - a;
}

void control_rod::update(double secs)
{
	update_rod();
	
	double m = (1 - absorbed / limit) * inserted;
	double r_fast = vals[val_t::N_FAST] * m;
	double r_slow = vals[val_t::N_SLOW] * m;

	vals[val_t::N_FAST] -= r_fast;
	vals[val_t::N_SLOW] -= r_slow;
	absorbed += r_fast + r_slow;
}

