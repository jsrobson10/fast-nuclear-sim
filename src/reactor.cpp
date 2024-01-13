
#include "reactor.hpp"

#include <iostream>

using namespace sim;

reactor::reactor(int size, fuel_rod fr)
{
	this->size = size;

	for(int y = 0; y < size; y++)
	for(int x = 0; x < size; x++)
	{
		rods.push_back({fr, x, y});
	}
}

static const double HEAT_K = 1.0/2.0;

void reactor::update(double secs)
{
	update_count++;
	
	static const int C[8][2] = {
		{0, -1},
		{0, 1},
		{-1, 0},
		{1, 0},
		{0, -1},
		{0, 1},
		{-1, 0},
		{1, 0}
	};

	const int C_OFF = update_count % 4;
	double heat_add[rods.size()] = {0};
	
	for(rod_t& r : rods)
	{
		r.fr.update(secs);

		for(int i = 0; i < 4; i++)
		{
			const auto [cx, cy] = C[C_OFF + i];
			int id_o = get_id(r.x + cx, r.y + cy);

			if(id_o == -1) continue;

			rod_t& r_o = rods[id_o];
			heat_add[id_o] += r.fr.extract_heat(HEAT_K, r_o.fr.get_temperature());
		}
	}

	for(int i = 0; i < rods.size(); i++)
	{
		rods[i].fr.add_heat(heat_add[i]);
	}
}

int reactor::get_id(int x, int y) const
{
	if(x < 0 || y < 0 || x >= size || y >= size) return -1;
	return y * size + x;
}

void reactor::display(std::ostream& o) const
{
	for(int y = -1; y <= size; y++)
	{
		for(int x = -1; x <= size; x++)
		{
			int id = get_id(x, y);

			o << "\t";

			if(id == -1) continue;

			o << rods[id].fr.get_temperature();
		}

		o << "\n";
	}
}

