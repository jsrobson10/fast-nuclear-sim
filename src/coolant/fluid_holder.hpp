
#pragma once

#include "fluid_t.hpp"

namespace sim::coolant
{

class fluid_holder
{
protected:

	const fluid_t fluid;
	const double volume; // litres

	double level = 0; // litres
	double steam = 0; // grams
	double heat = 0; // celsius
	
	fluid_holder(fluid_t fluid, double volume);

public:
	
	double add_heat(double m, double t);
	double add_fluid(double amount, double heat);
	double extract_fluid(double amount);
	virtual void add_steam(double amount, double t);

	constexpr double get_volume() const { return volume; } // litres
	constexpr double get_level() const { return level; } // litres
	constexpr double get_heat() const { return heat; } // celsius
	constexpr double get_steam() const { return steam; } // grams
	constexpr double get_steam_volume() const { return volume - level; } // litres
	constexpr double get_steam_density() const { return steam / get_steam_volume(); } // g/L
	constexpr double get_mass() const { return fluid.l_to_g(level) + steam; } // grams

	double get_pressure() const; // pascals
	void update(double dt);
};

};

