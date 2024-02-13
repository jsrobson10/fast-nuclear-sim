
#include "rod.hpp"
#include "reactor.hpp"

#include <cmath>

using namespace sim::reactor;

// Avogadro's Number
static double N_a = 6.02214076e23;

double rod::get(val_t type) const
{
	return vals[type];
}

void rod::add(val_t type, double v)
{
	vals[type] += v;
}

double rod::extract(val_t type, double s, double k, double o)
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

void rod::interact(rod* o, double secs)
{
	for(int i = 0; i < rod::VAL_N; i++)
	{
		val_t t = (val_t)i;
		double v = o->extract(t, secs, get_k(t), get(t));
		add(t, v);
		
		double v2 = std::abs(v / secs);
		o->vals_n[t] += v2;
		vals_n[t] += v2;
	}
}

double rod::get_flux() const
{
	return (vals_n[val_t::N_FAST] + vals_n[val_t::N_SLOW]) * N_a / (get_side_area() * 10000) / 4;
}

double rod::get_volume() const
{
	auto r = (sim::reactor::reactor*)reactor;
	return r->cell_width * r->cell_width * r->cell_height;
}

double rod::get_side_area() const
{
	auto r = (sim::reactor::reactor*)reactor;
	return r->cell_width * r->cell_height;
}

void rod::update_rod(double secs)
{
	// decay the free neutrons
	double m = std::pow(0.5, secs / 879.4);
	vals[val_t::N_FAST] *= m;
	vals[val_t::N_SLOW] *= m;

	// clear data
	for(int i = 0; i < rod::VAL_N; i++)
	{
		vals_n[(val_t)i] = 0;
	}
}

rod::operator Json::Value() const
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

