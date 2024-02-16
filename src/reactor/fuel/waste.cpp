
#include "waste.hpp"
#include "half_life.hpp"

using namespace Sim::Reactor::Fuel;

Waste::Waste(const Json::Value& node)
{
	const Json::Value& j_ladder = node["ladder"];
	
	for(int i = 0; i < N; i++)
	{
		high[i] = j_ladder[i][0].asDouble();
		low[i] = j_ladder[i][1].asDouble();
	}

	neutrons = node["neutrons"].asDouble();
	energy = node["energy"].asDouble();
}

Waste::operator Json::Value() const
{
	Json::Value node;
	Json::Value j_ladder;
	
	for(int i = 0; i < N; i++)
	{
		Json::Value j_step;
		j_step.append(high[i]);
		j_step.append(low[i]);
		j_ladder.append(std::move(j_step));
	}

	node["ladder"] = std::move(j_ladder);
	node["neutrons"] = neutrons;
	node["energy"] = energy;

	return node;
}

void Waste::update(double secs)
{
	double next[Waste::N - 1] = {0};
	double hl = 1;

	for(int i = 0; i < Waste::N - 1; hl *= 2, i++)
	{
		double m = 1 - half_life::get(secs, hl);
		double h = high[i] * m;
		double l = low[i] * m;

		next[i] += l + h;
		high[i] -= h;
		low[i] -= l;

		neutrons += h;
		energy += h + l;
	}

	for(int i = 0; i < Waste::N - 1; i++)
	{
		low[i + 1] += next[i];
	}
}

void Waste::add_fissile(double amount)
{
	double m = 0.5;

	for(int i = 0; i < Waste::N; i++)
	{
		high[i] += amount * m;
		m *= 0.5;
	}
}

double Waste::extract_neutrons()
{
	double v = neutrons;
	neutrons = 0;
	return v;
}

double Waste::extract_energy()
{
	double v = energy;
	energy = 0;
	return v;
}

