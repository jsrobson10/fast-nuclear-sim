
#include "rod.hpp"
#include "reactor.hpp"
#include "../util/math.hpp"
#include "../util/streams.hpp"

#include <cmath>

using namespace Sim::Reactor;

// Avogadro's Number
static double N_a = 6.02214076e23;

double Rod::get(val_t type) const
{
	return vals[type];
}

void Rod::add(val_t type, double v)
{
	vals[type] += v;
}

double Rod::extract(val_t type, double s, double k, double o)
{
	k *= get_k(type);

	double m = 1;
	k = 1 - k * get_k(type);
   
	if(k > 0)
	{
		m = 1 - std::pow(0.5, s * -std::log2(k));
	}

	double v = m * 0.5 * (get(type) - o);
	vals[type] -= v;

	return v;
}

void Rod::interact(Rod* o, double secs)
{
	for(int i = 0; i < Rod::VAL_N; i++)
	{
		val_t t = (val_t)i;
		double v = o->extract(t, secs, get_k(t), get(t));
		add(t, v);
		
		double v2 = std::abs(v / secs);
		o->vals_n[t] += v2;
		vals_n[t] += v2;
	}
}

glm::vec4 Rod::get_heat_colour() const
{
	double temp = vals[val_t::HEAT];

	if(temp < 0)
	{
		temp = 0;
	}

	// this should not happen
	if(std::isnan(temp))
	{
		return {1, 0, 1, 1};
	}

	if(temp < 120)
	{
		return {0, Util::Math::map(temp, 0, 120, 0, 1), 1, 1};
	}

	if(temp < 240)
	{
		return {0, 1, Util::Math::map(temp, 120, 240, 1, 0), 1};
	}

	if(temp < 280)
	{
		return {Util::Math::map(temp, 240, 280, 0, 1), 1, 0, 1};
	}

	if(temp < 320)
	{
		return {1, Util::Math::map(temp, 280, 320, 1, 0), 0, 1};
	}

	return {1, 0, 0, 1};
}

double Rod::get_flux() const
{
	return (vals_n[val_t::N_FAST] + vals_n[val_t::N_SLOW]) * N_a / (get_side_area() * 10000) / 4;
}

double Rod::get_volume() const
{
	auto r = (Sim::Reactor::Reactor*)reactor;
	return r->cell_width * r->cell_width * r->cell_height;
}

double Rod::get_side_area() const
{
	auto r = (Sim::Reactor::Reactor*)reactor;
	return r->cell_width * r->cell_height;
}

void Rod::update_rod(double secs)
{
//	// decay the free neutrons
//	double m = std::pow(0.5, secs / 879.4);
//	vals[val_t::N_FAST] *= m;
//	vals[val_t::N_SLOW] *= m;

	// clear data
	for(int i = 0; i < Rod::VAL_N; i++)
	{
		vals_n[(val_t)i] = 0;
	}
}

Rod::Rod(const Json::Value& node)
{
	const Json::Value& j_vals = node["vals"];
	
	selected = node["selected"].asBool();

	for(int i = 0; i < VAL_N; i++)
	{
		vals[i] = j_vals[i][0].asDouble();
		vals_n[i] = j_vals[i][1].asDouble();
	}
}

Json::Value Rod::serialize() const
{
	Json::Value node;
	Json::Value j_vals;
	
	for(int i = 0; i < VAL_N; i++)
	{
		Json::Value j_val;

		j_val.append(vals[i]);
		j_val.append(vals_n[i]);
		j_vals.append(std::move(j_val));
	}

	node["selected"] = selected;
	node["vals"] = std::move(j_vals);
	node["id"] = get_id();

	return node;
}

