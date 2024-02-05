
#pragma once

#include "vessel.hpp"
#include "../rod.hpp"

namespace sim::reactor::coolant
{

class pipe : public sim::reactor::rod
{
protected:
	
	coolant::vessel* vessel;
	double steam;
	
	virtual double get_k(sim::reactor::rod::val_t type) const;
	virtual const char* get_name() const { return "Coolant"; }
	virtual int get_id() const { return 2; }

	void update_pipe(double secs);

public:

	pipe(coolant::vessel* v);

	virtual std::unique_ptr<rod> clone() const { return std::make_unique<pipe>(*this); }
	virtual bool should_display() const { return true; }
	virtual void update(double secs);

};

}

