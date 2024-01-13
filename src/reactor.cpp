
#include "reactor.hpp"

#include <iostream>
#include <iomanip>

using namespace sim;

reactor::reactor(int size, fuel_rod fr)
{
	this->size = size;
	rods.resize(size * size, fr);

	rods[2].set_reactivity(1);
}

static const double HEAT_K = 1.0/64.0;

void reactor::update(double secs)
{
	for(int y = 0; y < size; y++)
	for(int x = 0; x < size; x++)
	{
		fuel_rod& fr = rods[get_id(x, y)];
		fr.update(secs);

		const int id_os[] = {
			get_id(x - 1, y),
			get_id(x, y - 1),
			get_id(x + 1, y),
			get_id(x, y + 1)
		};
		
		const double neutrons = fr.extract_free_neutrons();

		for(const int id_o : id_os)
		{
			if(id_o == -1) continue;


			fuel_rod& fr_o = rods[id_o];
			fr_o.add_heat(fr.extract_heat(HEAT_K * secs, fr_o.get_temperature()));
			fr_o.add_neutrons(neutrons / 4);
		}
	}
}

int reactor::get_id(int x, int y) const
{
	if(x < 0 || y < 0 || x >= size || y >= size) return -1;
	return y * size + x;
}

void reactor::display(std::ostream& o, int x, int y) const
{
	o << rods[get_id(x, y)];
}

