
#pragma once

#include "../coolant/pipe.hpp"

namespace Sim::Reactor::Control
{

class BoronRod : public Coolant::Pipe
{
	double inserted = 1;
	double absorbed = 0;
	
	virtual void display(std::ostream& o) const;
	virtual const char* get_name() const { return "Boron Control Rod"; }
	virtual glm::vec4 get_colour() const;
	virtual int get_id() const { return 5; }

public:
	
	BoronRod(Coolant::Vessel* v);
	BoronRod(const Json::Value& node, Coolant::Vessel* v);

	virtual Json::Value serialize() const;
	virtual void update(double secs);
	void set_reactivity(double a);
	double get_inserted() { return inserted; }
	
	virtual bool should_display() const { return true; }
	virtual bool should_select() const { return true; }
	virtual void update_selected(double a);
	virtual std::unique_ptr<Rod> clone() const { return std::make_unique<BoronRod>(*this); }
};

}

