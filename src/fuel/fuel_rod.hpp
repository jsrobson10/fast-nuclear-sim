
#pragma once

#include "sample.hpp"
#include "../rod.hpp"

namespace sim::fuel
{

class fuel_rod : public sim::rod
{
	sample s;
	
	virtual const char* get_name() const;
	virtual double get_k(sim::rod::val_t type) const;
	virtual void display(std::ostream& o) const;
	
public:

	fuel_rod(double fuel, double mass);
	
	virtual void update(double secs);
};

}

