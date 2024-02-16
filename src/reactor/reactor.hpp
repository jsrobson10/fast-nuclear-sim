
#pragma once

#include "rod.hpp"
#include "coolant/vessel.hpp"

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
	
	std::vector<std::unique_ptr<rod>> rods;

	double flux_rate = 0;
	double rod_speed = 0;
	int cursor;

	reactor(std::unique_ptr<rod>* rods, int width, int height, double cell_width, double cell_height);
	reactor(const Json::Value& node, coolant::vessel* v);
	reactor(const reactor& r);
	reactor(reactor&& r);
	
	void scram();
	void reset_rod_speed();
	void add_rod_speed(double a);
	void update(double secs);
	void get_stats(rod::val_t type, double& min, double& max);
	void get_rod_stats(int type, double& min, double& max);
	double get_flux();
	double get_energy_output();
	double get_average(rod::val_t type);
	double get_total(rod::val_t type);
	int move_cursor(int d);
	void toggle_selected();

	operator Json::Value() const;
	
private:

	void update_tile(double secs, int i, int x, int y);
	void update_interactions(int* rods_lookup, double secs);
	void update_selected(double dt);
};

}

