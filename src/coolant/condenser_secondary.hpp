
#pragma once

#include "fluid_holder.hpp"
#include "evaporator.hpp"
#include "condenser.hpp"

namespace sim::coolant
{

class condenser_secondary : public fluid_holder
{
	condenser* const primary;
	evaporator* const source;
	
public:

	condenser_secondary(condenser* primary, evaporator* source, double volume);

	virtual double add_heat(double m, double t) { return source->add_heat(m, t); }
	virtual void add_steam(double amount, double t) { return source->add_steam(amount, t); }
	virtual double extract_fluid(double amount) { return source->extract_fluid(amount); }

	virtual double add_fluid(double amount, double heat);
	
	virtual double get_volume() const { return source->get_volume(); }
	virtual double get_level() const { return source->get_level(); }
	virtual double get_heat() const { return source->get_heat(); } // celsius
	virtual double get_steam() const { return source->get_steam(); } // grams
	virtual double get_steam_volume() const { return source->get_steam_volume(); } // litres
	virtual double get_mass() const { return source->get_mass(); } // grams
	virtual double get_thermal_mass() const { return source->get_thermal_mass(); } // grams
	virtual double get_pressure() const { return source->get_pressure(); } // pascals
	virtual double get_steam_density() const { return source->get_steam_density(); } // g/L
	
	void update(double dt);
};

};

