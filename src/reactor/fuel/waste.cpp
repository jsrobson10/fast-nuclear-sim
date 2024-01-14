
#include "waste.hpp"
#include "half_life.hpp"

using namespace sim::reactor::fuel;

static const double SLOWDOWN_M = 1.0 / 4.0;

void waste::update(double secs)
{
	double hl = 1;
	double next_h[waste::N - 1] = {0};
	double next_l[waste::N - 1] = {0};

	for(int i = 0; i < waste::N - 1; i++)
	{
		double m = 1 - half_life::get(secs, hl);
		double h = high[i] * m;
		double l = low[i] * m;

		next_h[i] += h * (1 - SLOWDOWN_M);
		next_l[i] += l + h * SLOWDOWN_M;
		high[i] -= h;
		low[i] -= l;

		neutrons += h;
		energy += h + l;
		hl *= 2;
	}

	for(int i = 0; i < waste::N - 1; i++)
	{
		high[i + 1] += next_h[i];
		low[i + 1] += next_l[i];
	}
}

void waste::add_fissile(double amount)
{
	double m = 0.5;

	for(int i = 0; i < waste::N; i++)
	{
		high[i] += amount * m;
		m *= 0.5;
	}
}

double waste::extract_neutrons()
{
	double v = neutrons;
	neutrons = 0;
	return v;
}

double waste::extract_energy()
{
	double v = energy;
	energy = 0;
	return v;
}

