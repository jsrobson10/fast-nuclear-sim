
#include "heater.hpp"

using namespace sim::reactor::coolant;

void heater::update(double secs)
{
	update_rod(secs);

	vals_in[val_t::HEAT] += rate * secs;
}

void heater::update_selected(double a)
{
	rate += a;
}

double heater::get_k(sim::reactor::rod::val_t type) const
{
	return 0.5;
}

void heater::display(std::ostream& o) const
{
	o << "Rate: " << rate << "\n";
}

