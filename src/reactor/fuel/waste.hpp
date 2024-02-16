
#pragma once

#include <json/json.h>

namespace Sim::Reactor::Fuel
{

class Waste
{
	static const int N = 64;
	
	double high[N] = {0};
	double low[N] = {0};
	double neutrons = 0;
	double energy = 0;

public:
	
	constexpr Waste() { };

	Waste(const Json::Value& node);

	operator Json::Value() const;

	void update(double secs);
	void add_fissile(double amount);
	double extract_neutrons();
	double extract_energy();
};

}

