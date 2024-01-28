
#pragma once

#include <ostream>

#include "../../coolant/fluid_t.hpp"

namespace sim::reactor::coolant
{

class vessel
{
	double level; // litres
	double heat = 0; // celsius
	double steam = 0; // grams
	double steam_suspended = 0; // grams

public:
	
	const double height; // meters
	const double diameter; // meters
	const double volume; // litres
	const double bubble_hl; // seconds

	const sim::coolant::fluid_t fluid;

	vessel(double height, double diameter, double level, sim::coolant::fluid_t fluid);

	void update(double secs);
	double add_heat(double m, double t);
	double add_fluid(double amount, double heat);
	double extract_steam(double dt, double a, double p2);

	constexpr double get_volume() const { return volume; } // litres
	constexpr double get_level() const { return level; } // litres
	constexpr double get_heat() const { return heat; } // celsius
	constexpr double get_steam() const { return steam; } // grams
	constexpr double get_mass() const { return fluid.l_to_g(level) + steam; } // grams
	constexpr double get_steam_density() const { return steam / (volume - level); } // g/L
	constexpr double get_steam_suspended() const { return steam_suspended; } // grams
	constexpr double get_void_ratio() const { double s = steam_suspended / get_steam_density(); return s / (level + s); }
	
	double get_pressure() const; // pascals

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

}

