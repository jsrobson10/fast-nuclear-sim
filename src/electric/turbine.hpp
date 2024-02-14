
#pragma once

#include "../coolant/fluid_holder.hpp"
#include "../coolant/condenser.hpp"

namespace sim::electric
{

class turbine : public sim::coolant::fluid_holder
{
	coolant::condenser* const condenser;
	
	const double length;
	const double diameter;
	const double friction = 1;
	
	double velocity = 0; // m/s
	
public:

	turbine(coolant::fluid_t type, coolant::condenser* condenser, double length, double diameter, double mass);
	turbine(const Json::Value& node, coolant::condenser* condenser);
	
	void update(double dt);
	double get_rpm() const;
	
	virtual double add_heat(double m, double t) { return condenser->add_heat(m, t); }
	virtual double extract_fluid(double amount) { return condenser->extract_fluid(amount); }
	virtual double add_fluid(double amount, double heat) { return condenser->add_fluid(amount, heat); }

	virtual void add_gas(double steam, double gas, double t);

	virtual double get_volume() const { return condenser->get_volume(); }
	virtual double get_level() const { return condenser->get_level(); }
	virtual double get_heat() const { return condenser->get_heat(); } // celsius
	virtual double get_heat_k() const { return condenser->get_heat_k(); } // kelvin
	virtual double get_steam() const { return condenser->get_steam(); } // grams
	virtual double get_gas() const { return condenser->get_gas(); } // grams
	virtual double get_air() const { return condenser->get_air(); } // grams
	virtual double get_gas_volume() const { return condenser->get_gas_volume(); } // litres
	virtual double get_mass() const { return condenser->get_mass(); } // grams
	virtual double get_thermal_mass() const { return condenser->get_thermal_mass(); } // grams
	virtual double get_pressure() const { return condenser->get_pressure(); } // pascals
	virtual double get_gas_density() const { return condenser->get_gas_density(); } // g/L
	
	operator Json::Value() const;
};

};

