
#include "pipe.hpp"
#include "../reactor.hpp"

using namespace sim::reactor::coolant;

Pipe::Pipe(coolant::Vessel* v)
{
	this->vessel = v;
	this->steam = 0;
}

Pipe::Pipe(const Json::Value& node, coolant::Vessel* v) : vessel(v)
{
	steam = node["steam"].asDouble();
}

Json::Value Pipe::serialize() const
{
	Json::Value node(Rod::serialize());
	node["steam"] = steam;
	return node;
}

double Pipe::get_k(val_t type) const
{
	return vessel->get_level() / vessel->get_volume() * 0.5;
}

void Pipe::update(double secs)
{
	update_rod(secs);
	update_pipe(secs);
}

void Pipe::update_pipe(double secs)
{
	sim::reactor::Reactor* r = (sim::reactor::Reactor*)reactor;
	double m_heat = r->cell_width * r->cell_width * r->cell_height * 1e6;

	vals[val_t::HEAT] = vessel->add_heat(m_heat, vals[val_t::HEAT]);
	vals[val_t::N_SLOW] += vals[val_t::N_FAST] * (1 - vessel->get_void_ratio());
	vals[val_t::N_FAST] = 0;
}

