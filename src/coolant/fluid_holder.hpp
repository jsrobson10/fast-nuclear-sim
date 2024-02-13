
#pragma once

#include "fluid_t.hpp"
#include "../conversions/temperature.hpp"

namespace sim::coolant
{

class fluid_holder
{
		public://protected:

	double level = 0; // litres
	double steam = 0; // grams
	double heat = 0; // celsius

public:
	
	fluid_holder(fluid_t fluid, double volume, double extra_mass);
	
	const fluid_t fluid;
	const double volume; // litres
	const double extra_mass; // grams

	virtual double add_heat(double m, double t);
	virtual double extract_fluid(double amount);

	virtual double add_fluid(double amount, double heat);
	virtual void add_steam(double amount, double t);

	virtual double get_volume() const { return volume; } // litres
	virtual double get_level() const { return level; } // litres
	virtual double get_heat() const { return heat; } // celsius
	virtual double get_heat_k() const { return conversions::temperature::c_to_k(get_heat()); } // kelvin
	virtual double get_steam() const { return steam; } // grams
	virtual double get_steam_volume() const { return get_volume() - get_level(); } // litres
	virtual double get_mass() const { return fluid.l_to_g(get_level()) + get_steam(); } // grams
	virtual double get_thermal_mass() const { return get_mass() + extra_mass; } // grams
	virtual double get_pressure() const; // pascals
	virtual double get_steam_density() const; // g/L
	
	static double calc_pressure(double heat, double pressure, double mol);
	static double calc_pressure_mol(double heat, double pressure, double volume);
	
	void update_base(double dt);
};

};

