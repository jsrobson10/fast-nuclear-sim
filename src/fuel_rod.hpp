
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

	void update(double secs);
	void set_reactivity(double amount);

	double extract_free_neutrons();
	double extract_heat(double k, double o);

	friend std::ostream& operator<<(std::ostream& o, const fuel_rod& fr)
	{
		fr.display(o);
		return o;
	}
};

};

