
#pragma once

#include "rod.hpp"

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
	
	void update(double secs);
	void update_selected(int v);
	int move_cursor(int d);
	void toggle_selected();

private:

	void update_tile(double secs, int i, int x, int y);
	void update_interactions(int* rods_lookup, double secs);
};

}

