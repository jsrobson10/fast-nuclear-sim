
#pragma once

#include <ostream>

#include "../../coolant/fluid_t.hpp"

namespace sim::reactor::coolant
{

class vessel
{
	double level; // litres
	double heat = 0; // celcius
	double steam = 0; // grams

public:
	
	const double volume; // litres
	const sim::coolant::fluid_t fluid;

	vessel(double level, double volume, sim::coolant::fluid_t fluid);

	void update();
	double add_heat(double amount);
	double extract_steam(double dt, double a, double p2);

	constexpr double get_volume() const { return volume; }
	constexpr double get_level() const { return level; }
	constexpr double get_heat() const { return heat; }
	constexpr double get_steam() const { return steam; }
	
	double get_pressure() const;
};

}

std::ostream& operator<<(std::ostream& o, const sim::reactor::coolant::vessel& v);

