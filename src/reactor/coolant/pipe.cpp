
#include "pipe.hpp"
#include "../reactor.hpp"

using namespace sim::reactor::coolant;

pipe::pipe(coolant::vessel& v)
{
	this->vessel = &v;
	this->steam = 0;
}

double pipe::get_k(val_t type) const
{
	return vessel->get_level() / vessel->get_volume() * (1 - vessel->get_void_ratio()) * 0.5;
}

void pipe::update(double secs)
{
	update_rod(secs);
	update_pipe(secs);
}

void pipe::update_pipe(double secs)
{
	sim::reactor::reactor* r = (sim::reactor::reactor*)reactor;
	double m = r->cell_width * r->cell_width * r->cell_height * 1e6;

	vals[val_t::HEAT] = vessel->add_heat(m, vals[val_t::HEAT]);
	vals[val_t::N_SLOW] += vals[val_t::N_FAST];
	vals[val_t::N_FAST] = 0;
}

