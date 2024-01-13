
#pragma once

#include "fuel_rod.hpp"

#include <vector>

namespace sim
{

class reactor
{
	std::vector<fuel_rod> rods;

	int size;
	long update_count = 0;

	int get_id(int x, int y) const;

public:

	reactor(int radius, fuel_rod fr);

	void update(double secs);
	void display(std::ostream& o, int x, int y) const;

	constexpr int get_size() const { return size; }
};

}

