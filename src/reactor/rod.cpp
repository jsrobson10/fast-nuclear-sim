
#include "rod.hpp"

using namespace sim::reactor;

double rod::get(val_t type) const
{
	return vals[type];
}

void rod::add(val_t type, double v)
{
	vals_in[type] += v;
}

double rod::extract(val_t type, double k, double o)
{
	double v = k * get_k(type) * 0.5 * (get(type) - o);
	vals_in[type] -= v;
	return v;
}

void rod::interact(rod* o)
{
	for(int i = 0; i < rod::VAL_N; i++)
	{
		val_t v = (val_t)i;
		add(v, o->extract(v, get_k(v), get(v)));
	}
}

void rod::update_rod()
{
	for(int i = 0; i < rod::VAL_N; i++)
	{
		val_t v = (val_t)i;
		vals[v] += vals_in[v];
		vals_in[v] = 0;
	}
}

void rod::display(std::ostream& o) const
{

}

