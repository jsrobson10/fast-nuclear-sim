
#pragma once

#include "rod.hpp"

#include <random>
#include <iostream>
#include <memory>
#include <vector>

namespace sim::reactor
{

struct reactor
{
	const int width;
	const int height;
	const int size;

	std::unique_ptr<rod>* rods;
	int cursor = 0;

	reactor(std::unique_ptr<rod>* rods, int width, int height);
	reactor(const reactor& r);
	reactor(reactor&& r);
	~reactor();
	
	void update(std::mt19937& rand, double secs);
	void update_selected(int v);
	int move_cursor(int d);
	void toggle_selected();

private:

	void update_tile(std::mt19937& rand, double secs, int i, int x, int y);
	void update_interactions(std::mt19937& rand, int* rods_lookup, double secs);
};

}

