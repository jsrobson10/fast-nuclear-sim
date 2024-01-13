
#pragma once

#include "sample.hpp"

namespace sim
{

class fuel_rod
{
	sample fuel;
	
	double reactivity = 0;
	double temperature = 0;
	double neutrons_absorbed = 0;
	double neutrons_free = 0;

	void display(std::ostream& o) const;

public:

	fuel_rod(double fuel, double mass);
	fuel_rod() : fuel_rod(0, 0) {};

	void update(double secs);
	void set_reactivity(double amount);
	void add_neutrons(double amount);
	void add_heat(double amount);

	constexpr double get_temperature() const { return temperature; }
	constexpr double get_reactivity() const { return reactivity; }

	double extract_free_neutrons();
	double extract_heat(double k, double o);

	friend std::ostream& operator<<(std::ostream& o, const fuel_rod& fr)
	{
		fr.display(o);
		return o;
	}
};

};

