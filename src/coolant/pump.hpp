
#pragma once

#include <ostream>

namespace sim::coolant
{

template <class A>
class pump
{
	const double max;
	const double heat;
	
	A* a;
	double rate = 0;

public:

	constexpr pump(A& a, double max, double heat) : a(&a), max(max), heat(heat)
	{

	}

	constexpr double get_rate() const
	{
		return rate;
	}

	constexpr double get_flow() const
	{
		return rate * max;
	}

	constexpr void change_speed(double amount)
	{
		rate += amount;

		if(rate < 0) rate = 0;
		if(rate > 1) rate = 1;
	}

	void update(double secs)
	{
		a->add_fluid(rate * max * secs, heat);
	}

	friend std::ostream& operator<<(std::ostream& o, const pump& p)
	{
		o << "Rate: " << (p.get_rate() * 100) << " %\n";
		o << "Flow: " << (p.get_flow() * 0.001) << " kg/s\n";
		return o;
	}
};
	
};

