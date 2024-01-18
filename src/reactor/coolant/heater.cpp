
#include "heater.hpp"

using namespace sim::reactor::coolant;

void heater::update(double secs)
{
	update_rod(secs);

	vals[val_t::HEAT] += rate * secs;
}

void heater::update_selected(double a)
{
	rate += a;
}

void heater::display(std::ostream& o) const
{
	o << "Rate: " << rate << " C/s\n";
}

