
#include "pipe.hpp"
#include "../reactor.hpp"

using namespace sim::reactor::coolant;

pipe::pipe(coolant::vessel* v)
{
	this->vessel = v;
	this->steam = 0;
}

pipe::pipe(const Json::Value& node, coolant::vessel* v) : vessel(v)
{
	steam = node["steam"].asDouble();
}

Json::Value pipe::serialize() const
{
	Json::Value node(rod::serialize());
	node["steam"] = steam;
	return node;
}

double pipe::get_k(val_t type) const
{
	return vessel->get_level() / vessel->get_volume() * 0.5;
}

void pipe::update(double secs)
{
	update_rod(secs);
	update_pipe(secs);
}

void pipe::update_pipe(double secs)
{
	sim::reactor::reactor* r = (sim::reactor::reactor*)reactor;
	double m_heat = r->cell_width * r->cell_width * r->cell_height * 1e6;

	vals[val_t::HEAT] = vessel->add_heat(m_heat, vals[val_t::HEAT]);
	vals[val_t::N_SLOW] += vals[val_t::N_FAST] * (1 - vessel->get_void_ratio());
	vals[val_t::N_FAST] = 0;
}

