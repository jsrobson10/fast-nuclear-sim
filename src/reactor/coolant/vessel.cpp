
#include "vessel.hpp"

using namespace sim::reactor::coolant;

vessel::vessel(double level, double volume)
{
	this->level = level;
	this->volume = volume;
}

void vessel::update()
{
	level += level_in;
	steam += steam_in;
	level_in = 0;
	steam_in = 0;
}

double vessel::add_steam(double amount)
{
	double th = get_pressure();

	if(amount > th)
	{
		amount -= th;
		steam_in += amount;
		level_in -= amount;

		return th;
	}

	return amount;
}

void vessel::display(std::ostream& o) const
{
	o << "Volume: " << volume << "\n";
	o << "Level: " << level << "\n";
	o << "Steam: " << steam << "\n";
	o << "Pressure: " << get_pressure() << "\n";
}

