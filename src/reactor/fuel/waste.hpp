
#pragma once

namespace sim::reactor::fuel
{

class waste
{
	static const int N = 64;
	
	double high[N] = {0};
	double low[N] = {0};
	double neutrons = 0;
	double energy = 0;

public:

	void update(double secs);
	void add_fissile(double amount);
	double extract_neutrons();
	double extract_energy();
};

}
