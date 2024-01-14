
#include "pipe.hpp"

using namespace sim::reactor::coolant;

const char* pipe::get_name() const
{
	return "Coolant Pipe";
}

double pipe::get_k(val_t type) const
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

void pipe::update(double secs)
{
	update_rod();

	double v = vals[val_t::N_FAST] * 0.25;
	vals[val_t::N_FAST] -= v;
	vals[val_t::N_SLOW] += v;
}

