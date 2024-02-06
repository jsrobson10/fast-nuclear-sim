
#pragma once

#include "fluid_holder.hpp"

// the name isn't really important here, it's just to show that anything attached to this
// can kinda just do anything. for example, get rid of steam to the "outside", 
// or pump in new coolant. 

namespace sim::coolant
{

class sink : public fluid_holder
{
public:
	
	double heat;
	double pressure;
	double steam_density;

	sink(fluid_t type, double heat, double pressure, double steam_density);
	
	virtual double add_heat(double m, double t) { return t; }
	virtual double extract_fluid(double amount) { return amount; }
	virtual double add_fluid(double amount, double heat) { return amount; }
	virtual void add_steam(double amount, double t) { }
	virtual void update(double dt) { }

	virtual double get_pressure() const { return pressure; } // pascals
	virtual double get_steam_density() const { return steam_density; } // g/L
};

};

