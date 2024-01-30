
#pragma once

#include "rod.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace sim::reactor
{

struct reactor
{
	const double cell_width;
	const double cell_height;
	const int width;
	const int height;
	const int size;

	std::unique_ptr<std::unique_ptr<rod>[]> rods;
	int cursor = 0;

	reactor(std::unique_ptr<rod>* rods, int width, int height, double cell_width, double cell_height);
	reactor(const reactor& r);
	reactor(reactor&& r);
	
	void update(double secs);
	void update_selected(int v);
	void get_stats(rod::val_t type, double& min, double& max);
	void get_rod_stats(int type, double& min, double& max);
	double get_total(rod::val_t type);
	int move_cursor(int d);
	void toggle_selected();

private:

	void update_tile(double secs, int i, int x, int y);
	void update_interactions(int* rods_lookup, double secs);
};

}

