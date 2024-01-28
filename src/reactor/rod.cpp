
#include "rod.hpp"
#include "reactor.hpp"

#include <cmath>

using namespace sim::reactor;

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
		val_t v = (val_t)i;
		add(v, o->extract(v, secs, get_k(v), get(v)));
	}
}

double rod::get_speed() const
{
	int m = motion < 0 ? -1 : 1;
	return motion == 0 ? 0 : (std::pow(10, std::abs(motion)) * 1e-6 * m);
}

double rod::get_volume() const
{
	auto r = (sim::reactor::reactor*)reactor;
	return r->cell_width * r->cell_width * r->cell_height;
}

void rod::update_rod(double secs)
{
	// decay the free neutrons
	double m = std::pow(0.5, secs / 879.4);
	vals[val_t::N_FAST] *= m;
	vals[val_t::N_SLOW] *= m;

	if(motion != 0 && !is_selected())
	{
		motion = 0;
	}

	if(motion != 0)
	{
		update_selected(get_speed() * secs);
	}
}

void rod::update_rod_selected(int m)
{
	motion += m;

	if(motion > 5) motion = 5;
	if(motion < -5) motion = -5;
}

