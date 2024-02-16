
#include "heater.hpp"

using namespace sim::reactor::coolant;

Heater::Heater(const Json::Value& node) : Rod(node)
{
	rate = node["rate"].asDouble();
}

Json::Value Heater::serialize() const
{
	Json::Value node(Rod::serialize());
	node["rate"] = rate;
	return node;
}

void Heater::update(double secs)
{
	update_rod(secs);

	vals[val_t::HEAT] += rate * secs;
}

void Heater::update_selected(double a)
{
	rate += a;
}

void Heater::display(std::ostream& o) const
{
	o << "Rate: " << rate << " C/s\n";
}

