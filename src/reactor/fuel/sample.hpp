
#pragma once

#include "waste.hpp"

#include <ostream>

namespace Sim::Reactor::Fuel
{

class Sample
{
	constexpr static const double Xe_135_M = 1e6;

	Sim::Reactor::Fuel::Waste waste;
	
	// mol
	double fuel = 0;
	double i_135 = 0;
	double xe_135 = 0;
	double te_135 = 0;
	double u_238 = 0;
	double mass = 0;

	double energy = 0; // W
	double fast_neutrons = 0; // mol/s
	double slow_neutrons = 0; // mol/s
	double efficiency = 0;
	
	void display(std::ostream& o) const;
	void absorb_slow_neutrons(double secs);

public:

	Sample(double fuel);
	Sample(const Json::Value& node);

	operator Json::Value() const;
	
	void update(double secs);

	constexpr double get_fuel() const { return fuel; }
	constexpr double get_mass() const { return mass; }
	constexpr double get_energy() const { return energy; }
	constexpr double get_fast_neutrons() const { return fast_neutrons; }
	constexpr double get_slow_neutrons() const { return slow_neutrons; }
	constexpr double get_volume() const { return mass + xe_135 * Xe_135_M; }
	constexpr double get_efficiency() const { return efficiency; }
	constexpr double get_te_135() const { return te_135; }
	constexpr double get_i_135() const { return i_135; }
	constexpr double get_xe_135() const { return xe_135; }
	
	constexpr void clear_energy() { energy = 0; }
	constexpr void clear_fast_neutrons() { fast_neutrons = 0; }
	constexpr void clear_slow_neutrons() { slow_neutrons = 0; }
	constexpr void add_slow_neutrons(double a) { slow_neutrons += a; }

	friend std::ostream& operator<<(std::ostream& o, const Sample& s)
	{
		s.display(o);
		return o;
	}
};

}

