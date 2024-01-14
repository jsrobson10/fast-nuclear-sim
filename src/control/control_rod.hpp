
#pragma once

#include "../rod.hpp"

namespace sim::control
{

class control_rod : public sim::rod
{
	double inserted = 1;
	double absorbed = 0;
	double limit;
	
	virtual const char* get_name() const;
	virtual double get_k(sim::rod::val_t type) const;
	virtual void display(std::ostream& o) const;

public:

	control_rod(double limit);

	virtual void update(double secs);
	void set_reactivity(double a);
};

}

