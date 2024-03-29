
#include "graphite_rod.hpp"

#include <cmath>

using namespace Sim::Reactor::Control;

GraphiteRod::GraphiteRod(const Json::Value& node) : Rod(node)
{
	inserted = node["inserted"].asDouble();
}

Json::Value GraphiteRod::serialize() const
{
	Json::Value node(Rod::serialize());
	node["inserted"] = inserted;
	return node;
}

void GraphiteRod::display(std::ostream& o) const
{
	o << "Inserted: " << (inserted * 100) << "%\n";
};

glm::vec4 GraphiteRod::get_colour() const
{
	double v = inserted * 0.75 + 0.25;
	return {v, v, v, 1};
}

double GraphiteRod::get_k(val_t type) const
{
	if(type == val_t::HEAT) return 0.5;
	
	return inserted * 0.5;
}

void GraphiteRod::set_reactivity(double a)
{
	inserted = a;
}

void GraphiteRod::update(double secs)
{
	update_rod(secs);
	
	vals[val_t::N_SLOW] = vals[val_t::N_FAST];
	vals[val_t::N_FAST] = 0;
}

void GraphiteRod::update_selected(double a)
{
	inserted += a;

	if(inserted > 1) inserted = 1;
	if(inserted < 0) inserted = 0;
}

