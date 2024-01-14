
#pragma once

#include "../rod.hpp"

namespace sim::reactor::coolant
{

class pipe : public sim::reactor::rod
{
	virtual double get_k(sim::reactor::rod::val_t type) const;
	
	virtual const char* get_name() const { return "Coolant Pipe"; }
	virtual bool should_display() const { return true; }

public:

	virtual void update(double secs);
};

}

