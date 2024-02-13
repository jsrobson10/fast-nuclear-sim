
#pragma once

#include "fluid_holder.hpp"

namespace sim::coolant
{

class evaporator : public fluid_holder
{
	const double height;
	const double diameter;

	double steam_output = 0;
	
public:

	evaporator(fluid_t type, double height, double diameter, double mass, double level);
	evaporator(const Json::Value& node);
	
	operator Json::Value() const;

	double get_steam_output();
	void update(double dt);
};

};

