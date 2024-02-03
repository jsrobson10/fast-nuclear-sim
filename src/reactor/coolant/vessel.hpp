
#pragma once

#include <ostream>

#include "../../coolant/fluid_holder.hpp"

namespace sim::reactor::coolant
{

class vessel : public sim::coolant::fluid_holder
{
public:
	
	const double height; // meters
	const double diameter; // meters
	double steam_suspended = 0; // grams

	vessel(sim::coolant::fluid_t fluid, double height, double diameter, double level);

	double get_steam_suspended() const; // grams
	double get_void_ratio() const;
	double get_bubble_hl() const;

	void update(double secs);

	friend std::ostream& operator<<(std::ostream& o, const vessel& v)
	{
		o << "Volume: " << v.get_volume() << " L\n";
		o << "Level: " << v.get_level() << " L\n";
		o << "Steam: " << v.get_steam() << " g\n";
		o << "Heat: " << v.get_heat() << " C\n";
		o << "Pressure: " << (v.get_pressure() * 0.001) << " kPa\n";
		o << "Void Ratio: " << (v.get_void_ratio() * 100) << " %\n";
	
		return o;
	}

};

};

