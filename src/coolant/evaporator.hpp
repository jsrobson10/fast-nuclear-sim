
#pragma once

#include "fluid_holder.hpp"

namespace Sim::Coolant
{

class Evaporator : public FluidHolder
{
	double steam_output = 0;
	
public:

	Evaporator(Fluid type, double volume, double mass, double level);
	Evaporator(Fluid type, double height, double diameter, double mass, double level);
	Evaporator(const Json::Value& node);
	
	operator Json::Value() const;

	double get_steam_output();
	void update(double dt);
};

};

