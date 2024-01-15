
#include "pipe.hpp"

using namespace sim::reactor::coolant;

pipe::pipe(coolant::vessel& v)
{
	this->vessel = &v;
	this->steam = 0;
}

void pipe::display(std::ostream& o) const
{
	o << "Steam: +" << steam  << "\n";
}

double pipe::get_k(val_t type) const
{
	return vessel->get_level() / vessel->get_volume();
}

void pipe::update(double secs)
{
	double v;
	
	update_rod(secs);
	
	v = vessel->add_steam(vals[val_t::HEAT]);
	steam = vals[val_t::HEAT] - v;
	vals[val_t::HEAT] = v;

	v = vals[val_t::N_FAST];
	vals[val_t::N_FAST] -= v;
	vals[val_t::N_SLOW] += v;
}

