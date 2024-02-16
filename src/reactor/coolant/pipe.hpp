
#pragma once

#include "vessel.hpp"
#include "../rod.hpp"

namespace Sim::Reactor::coolant
{

class Pipe : public Sim::Reactor::Rod
{
protected:
	
	coolant::Vessel* vessel;
	double steam;
	
	virtual double get_k(Sim::Reactor::Rod::val_t type) const;
	virtual const char* get_name() const { return "Coolant"; }
	virtual int get_id() const { return 2; }

	void update_pipe(double secs);

public:

	Pipe(coolant::Vessel* v);
	Pipe(const Json::Value& node, coolant::Vessel* v);

	virtual Json::Value serialize() const;
	virtual std::unique_ptr<Rod> clone() const { return std::make_unique<Pipe>(*this); }
	virtual bool should_display() const { return true; }
	virtual void update(double secs);

};

}

