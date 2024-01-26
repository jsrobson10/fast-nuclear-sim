
#include "reactor.hpp"

#include <algorithm>

using namespace sim::reactor;

reactor::reactor(std::unique_ptr<rod>* rods, int width, int height) : width(width), height(height), size(width * height)
{
	this->rods = new std::unique_ptr<rod>[width * height];

	for(int i = 0; i < size; i++)
	{
		this->rods[i] = std::move(rods[i]);
	}
}

reactor::reactor(reactor&& o) : width(o.width), height(o.height), size(o.size)
{
	rods = o.rods;
	cursor = o.cursor;
	o.rods = nullptr;
}

reactor::~reactor()
{
	if(rods != nullptr)
	{
		delete[] rods;
	}
}

void reactor::update(std::mt19937& rand, double secs)
{
	int rods_lookup[size];

	for(int i = 0; i < size; i++)
	{
		rods_lookup[i] = i;
	}

	for(int i = 0; i < size; i++)
	{
		rods[i]->update(secs);
	}

	update_interactions(rand, rods_lookup, secs / 2);
}

void reactor::update_selected(int v)
{
	for(int i = 0; i < size; i++)
	{
		rod* r = rods[i].get();

		if(r->is_selected())
		{
			r->update_rod_selected(v);
		}
	}
}

int reactor::move_cursor(int d)
{
	for(int i = 0; i < size; i++)
	{
		cursor = (cursor + d) % size;
		
		if(cursor < 0)
		{
			cursor += size;
		}

		if(rods[cursor]->should_select())
		{
			return cursor;
		}
	}

	return 0;
}

void reactor::toggle_selected()
{
	if(rods[cursor]->should_select())
	{
		rods[cursor]->toggle_selected();
	}
}

void reactor::update_tile(std::mt19937& rand, double secs, int i, int x, int y)
{
	int nb_lookup[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	std::shuffle(nb_lookup, &nb_lookup[3], rand);

	for(int j = 0; j < 4; j++)
	{
		int xp = x + nb_lookup[j][0];
		int yp = y + nb_lookup[j][1];
		
		if(xp >= 0 && yp >= 0 && xp < width && yp < height)
		{
			rods[i]->interact(rods[yp * width + xp].get(), secs / 2);
		}
	}
}

void reactor::update_interactions(std::mt19937& rand, int* rods_lookup, double secs)
{
	std::shuffle(rods_lookup, &rods_lookup[size - 1], rand);

	for(int id = 0; id < size; id++)
	{
		int i = rods_lookup[id];
		int x = i % width;
		int y = i / width;

		for(int j = 0; j < 4; j++)
		{
			update_tile(rand, secs, i, x, y);
		}
	}
}
