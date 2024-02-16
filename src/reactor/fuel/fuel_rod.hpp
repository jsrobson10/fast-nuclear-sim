
#pragma once

#include "sample.hpp"
#include "../rod.hpp"

namespace Sim::Reactor::fuel
{

class FuelRod : public Sim::Reactor::Rod
{
	Sample s;
	
	virtual double get_k(val_t type) const { return 0.5; }
	virtual void display(std::ostream& o) const;
	
	virtual bool has_sensors(val_t t) const { return true; }
	virtual const char* get_name() const { return "Fuel"; }
	virtual int get_id() const { return 1; }
	virtual double get_energy_output() const;
	
public:

	FuelRod(double fuel);
	FuelRod(const Json::Value& node);
	
	virtual Json::Value serialize() const;
	virtual std::unique_ptr<Rod> clone() const { return std::make_unique<FuelRod>(*this); }
	virtual bool should_display() const { return true; }
	virtual void update(double secs);
};

}

