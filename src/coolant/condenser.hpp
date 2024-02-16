
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class Condenser : public FluidHolder
{
	const double height;
	const double diameter;

public:

	Condenser(Fluid type, double height, double diameter, double mass, double level);
	Condenser(const Json::Value& node);
	
	void update(double dt);

	operator Json::Value() const;
};

};

