
#pragma once

#include "../rod.hpp"

namespace sim::coolant
{

class pipe : public sim::rod
{
	virtual const char* get_name() const;
	virtual double get_k(sim::rod::val_t type) const;

public:

	virtual void update(double secs);
};

}

