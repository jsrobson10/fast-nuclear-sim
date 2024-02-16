
#pragma once

#include "../rod.hpp"

namespace Sim::Reactor::Coolant
{

class Heater : public Sim::Reactor::Rod
{
	double rate = 0;

	virtual void display(std::ostream& o) const;

	virtual bool has_sensors(val_t t) const { return true; }
	virtual const char* get_name() const { return "Heater"; }
	virtual double get_k(val_t type) const { return 0.5; }
	virtual int get_id() const { return 3; }

public:
	
	constexpr Heater() {}
	
	Heater(const Json::Value& node);

	virtual Json::Value serialize() const;
	virtual void update(double secs);
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
	virtual std::unique_ptr<Rod> clone() const { return std::make_unique<Heater>(*this); }
};

};

