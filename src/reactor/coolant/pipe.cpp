
#include "pipe.hpp"

using namespace sim::reactor::coolant;

pipe::pipe(coolant::vessel& v)
{
	this->vessel = &v;
	this->steam = 0;
}

double pipe::get_k(val_t type) const
{
	return vessel->get_level() / vessel->get_volume() * 0.5;
}

void pipe::update(double secs)
{
	update_rod(secs);
	update_pipe(secs);
}

void pipe::update_pipe(double secs)
{
	vals[val_t::HEAT] = vessel->add_heat(vals[val_t::HEAT]);
	vals[val_t::N_SLOW] += vals[val_t::N_FAST];
	vals[val_t::N_FAST] = 0;
}

