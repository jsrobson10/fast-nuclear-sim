
#include "waste.hpp"
#include "half_life.hpp"

using namespace sim::fuel;

void waste::update(double secs)
{
	double hl = 1;
	double next[waste::N - 1] = {0};

	for(int i = 0; i < waste::N - 1; i++)
	{
		double m = 1 - half_life::get(secs, hl);
		double h = high[i] * m;
		double l = low[i] * m;

		next[i] += h + l;
		high[i] -= h;
		low[i] -= l;

		neutrons += h;
		energy += h + l;
		hl *= 2;
	}

	for(int i = 0; i < waste::N - 1; i++)
	{
		low[i + 1] += next[i];
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

