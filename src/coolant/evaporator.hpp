
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class Evaporator : public FluidHolder
{
	const double height;
	const double diameter;

	double steam_output = 0;
	
public:

	Evaporator(Fluid type, double height, double diameter, double mass, double level);
	Evaporator(const Json::Value& node);
	
	operator Json::Value() const;

	double get_steam_output();
	void update(double dt);
};

};

