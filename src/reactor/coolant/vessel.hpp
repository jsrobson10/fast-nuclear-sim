
#pragma once

#include <ostream>

namespace sim::reactor::coolant
{

class vessel
{
	double volume;
	double level;
	
	double steam = 0;
	double level_in = 0;
	double steam_in = 0;

	void display(std::ostream& o) const;

public:

	vessel(double level, double volume);

	void update();
	double add_steam(double amount);

	constexpr double get_volume() const { return volume; }
	constexpr double get_level() const { return level; }
	constexpr double get_steam() const { return steam; }
	constexpr double get_pressure() const { return steam / (volume - level); }

	friend std::ostream& operator<<(std::ostream& o, const vessel& v)
	{
		v.display(o);
		return o;
	}
};

}

