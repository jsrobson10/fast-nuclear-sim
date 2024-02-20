
#pragma once

#include <vector>

#include <json/json.h>

namespace Sim::Electric
{

class Grid
{
	double phase = 0;
	double power_output = 0;	// W
	double energy_output = 0;	// J
	
public:

	const double frequency = 60;
	const double power_limit = 1e9;			// W
	const double output_resistance = 10;	// Ohms

	constexpr Grid() {}

	Grid(const Json::Value& node);
	operator Json::Value() const;

	void update(double dt);
	void pull_energy(double joules);
	double get_light_intensity();
	bool is_above_limit();

	constexpr double get_phase() { return phase; }
	constexpr double get_power_output() { return power_output; }
};

};

