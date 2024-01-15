
#pragma once

#include "vessel.hpp"
#include "../rod.hpp"

namespace sim::reactor::coolant
{

class pipe : public sim::reactor::rod
{
	coolant::vessel* vessel;
	double steam;
	
	virtual double get_k(sim::reactor::rod::val_t type) const;
	
	virtual const char* get_name() const { return "Coolant"; }
	virtual void display(std::ostream& o) const;

public:

	pipe(coolant::vessel& v);

	virtual bool should_display() const { return true; }

	virtual void update(double secs);
};

}

