
#pragma once

#include <ostream>

#include "../../coolant/fluid_holder.hpp"

namespace Sim::Reactor::Coolant
{

class Vessel : public Sim::Coolant::FluidHolder
{
public:
	
	const double height; // meters
	const double diameter; // meters
	const double bubble_hl; // seconds

	double steam_suspended = 0; // grams

	Vessel(Sim::Coolant::Fluid fluid, double height, double diameter, double mass, double level, double bubble_hl);
	Vessel(const Json::Value& node);

	double get_steam_suspended() const; // grams
	double get_void_ratio() const;

	void update(double secs);
	
	operator Json::Value() const;

	friend std::ostream& operator<<(std::ostream& o, const Vessel& v)
	{
		o << "Volume: " << v.get_volume() << " L\n";
		o << "Level: " << v.get_level() << " L\n";
		o << "Steam: " << v.get_steam() << " g\n";
		o << "Heat: " << v.get_heat() << " C\n";
		o << "Pressure: " << (v.get_pressure() * 0.001) << " kPa\n";
		o << "Void Ratio: " << (v.get_void_ratio() * 100) << " %\n";
	
		return o;
	}

};

};

