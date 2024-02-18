
#pragma once

#include "../coolant/fluid_holder.hpp"
#include "../coolant/condenser.hpp"

namespace Sim::Electric
{

class Turbine : public Sim::Coolant::FluidHolder
{
	Coolant::Condenser* const condenser;
	
	const double length;
	const double diameter;
	const double friction = 1e5; // J/rev
	
	double energy_input = 0; // J
	double energy_generated = 0; // W
	double velocity = 0; // m/s
	double phase = 0;

	void set_rpm(double rpm);
	
public:
	
	bool breaker_closed = false;
	bool is_stable = false;

	Turbine(Coolant::Fluid type, Coolant::Condenser* condenser, double length, double diameter, double mass);
	Turbine(const Json::Value& node, Coolant::Condenser* condenser);
	
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
	
	constexpr double get_energy_generated() const { return energy_generated; }
	constexpr double get_phase() const { return phase; }
	double get_phase_diff() const;
	
	operator Json::Value() const;
};

};

