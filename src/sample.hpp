
#pragma once

#include "waste.hpp"

#include <ostream>

namespace sim
{

class sample
{
	sim::waste waste;
	
	double fuel = 0;
	double i_135 = 0;
	double xe_135 = 0;
	double te_135 = 0;
	double mass = 0;

	double energy = 0;
	double fast_neutrons = 0;
	double slow_neutrons = 0;
	
	void display(std::ostream& o) const;

public:

	sample(double fuel, double mass);

	void update(double secs);
	double extract_energy();
	double extract_fast_neutrons();
	void add_slow_neutrons(double a);
	
	double get_volume() const;
	double get_mass() const;

	friend std::ostream& operator<<(std::ostream& o, const sample& s)
	{
		s.display(o);
		return o;
	}
};


}

