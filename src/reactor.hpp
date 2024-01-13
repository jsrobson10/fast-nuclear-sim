
#pragma once

#include "fuel_rod.hpp"

#include <vector>

namespace sim
{

class reactor
{
	struct rod_t
	{
		fuel_rod fr;
		int x, y;

		rod_t(fuel_rod fr, int x, int y) : fr(fr) {}
	};
	
	std::vector<rod_t> rods;

	int size;
	long update_count = 0;

	int get_id(int x, int y) const;
	void display(std::ostream& o) const;

public:

	reactor(int radius, fuel_rod fr);

	void update(double secs);

	friend std::ostream& operator<<(std::ostream& o, reactor& r)
	{
		r.display(o);
		return o;
	}
};

}

