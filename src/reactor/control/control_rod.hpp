
#pragma once

#include "../coolant/pipe.hpp"

namespace sim::reactor::control
{

class control_rod : public sim::reactor::coolant::pipe
{
	const double limit;
	const double max;

	double inserted = 1;
	double absorbed = 0;
	
	virtual void display(std::ostream& o) const;

	virtual const char* get_name() const { return "Control Rod"; }
	virtual rod* clone() const { return new control_rod(*this); };

public:

	control_rod(coolant::vessel& v, double limit, double max);

	virtual void update(double secs);
	void set_reactivity(double a);
	
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
};

}

