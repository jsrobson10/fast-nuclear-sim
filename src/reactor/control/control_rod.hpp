
#pragma once

#include "../rod.hpp"

namespace sim::reactor::control
{

class control_rod : public sim::reactor::rod
{
	double inserted = 1;
	double absorbed = 0;
	double limit;
	double max;
	
	virtual double get_k(sim::reactor::rod::val_t type) const;
	virtual void display(std::ostream& o) const;

	virtual const char* get_name() const { return "Control Rod"; }

public:

	control_rod(double limit, double max);

	virtual void update(double secs);
	void set_reactivity(double a);
	
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
};

}

