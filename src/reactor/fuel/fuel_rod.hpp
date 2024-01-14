
#pragma once

#include "sample.hpp"
#include "../rod.hpp"

namespace sim::reactor::fuel
{

class fuel_rod : public sim::reactor::rod
{
	sample s;
	
	virtual const char* get_name() const;
	virtual double get_k(sim::reactor::rod::val_t type) const;
	virtual void display(std::ostream& o) const;
	
public:

	fuel_rod(double fuel, double mass);
	
	virtual void update(double secs);
};

}

