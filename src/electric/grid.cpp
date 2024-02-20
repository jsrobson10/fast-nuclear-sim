
#include "grid.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Electric;

Grid::Grid(const Json::Value& node) :
		frequency(node["frequency"].asDouble()),
		power_limit(node["power_limit"].asDouble())
{
	phase = node["phase"].asDouble();
	energy_output = node["energy_output"].asDouble();
	power_output = node["power_output"].asDouble();
}

Grid::operator Json::Value() const
{
	Json::Value node;
	node["phase"] = phase;
	node["frequency"] = frequency;
	node["energy_output"] = energy_output;
	node["power_output"] = power_output;
	node["power_limit"] = power_limit;
	return node;
}

void Grid::update(double dt)
{
	phase = std::fmod(phase + 60.0 * 2*M_PI * dt, 2*M_PI);
	power_output = energy_output / dt;
	energy_output = 0;
}

void Grid::pull_energy(double joules)
{
	energy_output -= joules;
}

double Grid::get_light_intensity()
{
	if(energy_output < 0)
	{
		return std::max(0.0, 1 + energy_output / 1e8);
	}

	return 1;
}

bool Grid::is_above_limit()
{
	return std::abs(power_output) > power_limit;
}

