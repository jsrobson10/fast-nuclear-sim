
#pragma once

#include "sample.hpp"
#include "../rod.hpp"

namespace sim::reactor::fuel
{

class fuel_rod : public sim::reactor::rod
{
	sample s;
	
	virtual double get_k(val_t type) const { return 0.5; }
	virtual void display(std::ostream& o) const;
	
	virtual bool has_sensors(val_t t) const { return true; }
	virtual const char* get_name() const { return "Fuel"; }
	virtual int get_id() const { return 1; }
	virtual double get_energy_output() const;
	virtual glm::vec4 get_colour() const;
	
public:

	fuel_rod(double fuel);
	
	virtual std::unique_ptr<rod> clone() const { return std::make_unique<fuel_rod>(*this); }
	virtual bool should_display() const { return true; }
	virtual void update(double secs);
};

}

