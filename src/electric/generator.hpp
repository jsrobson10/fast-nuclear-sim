
#pragma once

#include "grid.hpp"
#include "turbine.hpp"

namespace Sim::Electric
{

class Generator
{
	Grid* const grid;
	Turbine* const turbine;
	
	const double length; // m
	const double diameter; // m
	const double friction = 1e5; // J/rev
	const double mass; // g

	double energy_generated = 0; // W
	double velocity = 0; // m/s
	double phase = 0;
	
	void set_rpm(double rpm);
	
public:
	
	bool breaker_closed = false;
	bool is_stable = false;

	Generator(Turbine* turbine, Grid* grid, double length, double diameter, double mass);
	Generator(const Json::Value& node, Turbine* turbine, Grid* grid);

	void update(double dt);
	
	double get_rpm() const;
	double get_phase_diff() const;
	
	operator Json::Value() const;
	
	constexpr double get_energy_generated() const { return energy_generated; }
	constexpr double get_phase() const { return phase; }
};

};

