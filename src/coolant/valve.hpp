
#pragma once

namespace sim::coolant
{

template <class A>
class valve
{
	A* a;
	
	const double max;
	const double pressure;

	double state = 0;
	double rate = 0;

public:

	constexpr valve(A& a, double max, double pressure) : a(&a), max(max), pressure(pressure)
	{

	}

	constexpr double get_state() const
	{
		return state;
	}

	constexpr void set_state(double v)
	{
		if(v > 1) v = 1;
		if(v < 0) v = 0;
		state = v;
	}

	constexpr void open(double v)
	{
		set_state(state + v);
	}

	constexpr void update(double secs)
	{
		rate = a->extract_steam(secs, state * max, pressure) / secs;
	}

	friend std::ostream& operator<<(std::ostream& o, const valve& v)
	{
		o << "Opened: " << (v.state * 100) << " %\n";
		o << "Rate: " << v.rate << " g/s\n";
		return o;
	}
};

};

