
#pragma once

#include "waste.hpp"

#include <ostream>

namespace sim::reactor::fuel
{

class sample
{
	constexpr static const double Xe_135_M = 1e3;

	sim::reactor::fuel::waste waste;
	
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
	
	constexpr double get_fuel() const { return fuel; }
	constexpr double get_mass() const { return mass; }
	constexpr double get_energy() const { return energy; }
	constexpr double get_volume() const { return mass + xe_135 * Xe_135_M; }
	constexpr double get_efficiency() const { return fuel / get_volume(); }

	friend std::ostream& operator<<(std::ostream& o, const sample& s)
	{
		s.display(o);
		return o;
	}
};

}

