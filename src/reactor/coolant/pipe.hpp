
#pragma once

#include "../rod.hpp"

namespace sim::reactor::coolant
{

class pipe : public sim::reactor::rod
{
	virtual const char* get_name() const;
	virtual double get_k(sim::reactor::rod::val_t type) const;

public:

	virtual void update(double secs);
};

}

