
#pragma once

#include "../rod.hpp"

namespace sim::reactor::coolant
{

class heater : public sim::reactor::rod
{
	double rate = 0;

	virtual double get_k(sim::reactor::rod::val_t type) const;
	virtual void display(std::ostream& o) const;

	virtual const char* get_name() const { return "Heater"; }

public:

	virtual void update(double secs);
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
};

};

