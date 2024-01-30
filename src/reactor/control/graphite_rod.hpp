
#pragma once

#include "../rod.hpp"

namespace sim::reactor::control
{

class graphite_rod : public sim::reactor::rod
{
	double inserted = 0;
	
	virtual void display(std::ostream& o) const;
	virtual const char* get_name() const { return "Graphite Rod"; }
	virtual double get_k(val_t type) const;
	virtual glm::vec4 get_colour() const;
	virtual int get_id() const { return 4; }
	
	double get_inserted() { return inserted; }

public:

	virtual void update(double secs);
	void set_reactivity(double a);
	
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
	virtual std::unique_ptr<rod> clone() const { return std::make_unique<graphite_rod>(*this); }
};

}

