
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
	double v;
	
	update_rod(secs);
	
	v = vessel->add_heat(vals[val_t::HEAT]);
	steam = vals[val_t::HEAT] - v;
	vals[val_t::HEAT] = v;

	v = vals[val_t::N_FAST];
	vals_in[val_t::N_FAST] -= v;
	vals_in[val_t::N_SLOW] += v;
}

