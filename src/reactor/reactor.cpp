
#include "reactor.hpp"
#include "../util/random.hpp"
#include "builder.hpp"

#include <algorithm>

using namespace Sim::Reactor;

Reactor::Reactor(std::unique_ptr<Rod>* rods, int w, int h, double cw, double ch) : cell_width(cw), cell_height(ch), width(w), height(h), size(w * h)
{
	this->rods = std::vector<std::unique_ptr<Rod>>(w * h);
	this->cursor = w * h;

	for(int i = 0; i < size; i++)
	{
		this->rods[i] = std::move(rods[i]);
		this->rods[i]->reactor = this;
	}
}

Reactor::Reactor(Reactor&& o) : cell_width(o.cell_width), cell_height(o.cell_height), width(o.width), height(o.height), size(o.size)
{
	rods = std::move(o.rods);
	cursor = o.cursor;

	for(int i = 0; i < size; i++)
	{
		rods[i]->reactor = this;
	}
}

Reactor::Reactor(const Reactor& o) : cell_width(o.cell_width), cell_height(o.cell_height), width(o.width), height(o.height), size(o.size)
{
	rods = std::vector<std::unique_ptr<Rod>>(width * height);
	cursor = o.cursor;

	for(int i = 0; i < size; i++)
	{
		rods[i] = std::unique_ptr<Rod>(o.rods[i]->clone());
		rods[i]->reactor = this;
	}
}

void Reactor::reset_rod_speed()
{
	rod_speed = 0;
}

void Reactor::add_rod_speed(double a)
{
	rod_speed -= a;

	if(rod_speed < -0.2)
		rod_speed = -0.2;
	if(rod_speed > 0.2)
		rod_speed = 0.2;
}

void Reactor::scram()
{
	rod_speed = -0.2;

	for(int i = 0; i < size; i++)
	{
		if(rods[i]->should_select())
		{
			rods[i]->selected = true;
		}
	}
}

void Reactor::update(double secs)
{
	int rods_lookup[size];
	double temp_min, temp_max;
	double flux_initial = get_flux();

	get_stats(Rod::val_t::HEAT, temp_min, temp_max);

	if(temp_max > 360)
	{
		scram();
	}

	for(int i = 0; i < size; i++)
	{
		rods_lookup[i] = i;
	}

	for(int i = 0; i < size; i++)
	{
		rods[i]->update(secs);
	}

	update_interactions(rods_lookup, secs / 2);

	if(std::abs(rod_speed) < 1e-6)
	{
		rod_speed = 0;
	}

	if(rod_speed != 0)
	{
		update_selected(secs);
	}

	if(flux_initial > 0)
	{
		flux_rate = (get_flux() - flux_initial) / flux_initial / secs;
	}
}

void Reactor::update_selected(double dt)
{
	for(int i = 0; i < size; i++)
	{
		Rod* r = rods[i].get();

		if(r->selected)
		{
			r->update_selected(rod_speed * dt);
		}
	}
}

int Reactor::move_cursor(int d)
{
	goto logic;
	
	while(cursor != size && !rods[cursor]->should_display())
	{
logic:	cursor = (cursor + d) % (size + 1);
	
		if(cursor < 0)
		{
			cursor += size + 1;
		}

		if(d > 1) d = 1;
		if(d < -1) d = -1;
	}

	return cursor;
}

void Reactor::toggle_selected()
{
	if(cursor < size && rods[cursor]->should_select())
	{
		rods[cursor]->toggle_selected();
	}
}

void Reactor::update_tile(double secs, int i, int x, int y)
{
	int nb_lookup[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
	std::shuffle(nb_lookup, &nb_lookup[3], Util::Random::gen);

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

void Reactor::update_interactions(int* rods_lookup, double secs)
{
	std::shuffle(rods_lookup, &rods_lookup[size - 1], Util::Random::gen);

	for(int id = 0; id < size; id++)
	{
		int i = rods_lookup[id];
		int x = i % width;
		int y = i / width;

		for(int j = 0; j < 4; j++)
		{
			update_tile(secs, i, x, y);
		}
	}
}

double Reactor::get_total(Rod::val_t type)
{
	double v = 0;

	for(int i = 0; i < size; i++)
	{
		v += rods[i]->get(type);
	}

	return v;
}

double Reactor::get_average(Rod::val_t type)
{
	return get_total(type) / size;
}

double Reactor::get_flux()
{
	double v = 0;

	for(int i = 0; i < size; i++)
	{
		v += rods[i]->get_flux();
	}
	
	return v / size;
}

double Reactor::get_energy_output()
{
	double v = 0;

	for(int i = 0; i < size; i++)
	{
		v += rods[i]->get_energy_output();
	}

	return v;
}

void Reactor::get_stats(Rod::val_t type, double& min, double& max)
{
	min = INFINITY;
	max = -INFINITY;

	for(int i = 0; i < size; i++)
	{
		if(!rods[i]->has_sensors(type))
		{
			continue;
		}

		double v = rods[i]->get(type);

		if(v > max) max = v;
		if(v < min) min = v;
	}
}

Reactor::Reactor(const Json::Value& node, coolant::Vessel* v) :
		cell_width(node["cell_width"].asDouble()),
		cell_height(node["cell_height"].asDouble()),
		width(node["width"].asInt()),
		height(node["height"].asInt()),
		size(node["size"].asInt())
{
	const Json::Value& j_rods = node["rods"];
	
	rod_speed = node["rod_speed"].asDouble();
	cursor = node["cursor"].asInt();

	for(int i = 0; i < size; i++)
	{
		std::unique_ptr<Rod> r = load_rod(j_rods[i], v);
		r->reactor = this;

		rods.push_back(std::move(r));
	}
}

Reactor::operator Json::Value() const
{
	Json::Value node;

	node["cell_width"] = cell_width;
	node["cell_height"] = cell_height;
	node["width"] = width;
	node["height"] = height;
	node["size"] = size;
	node["rod_speed"] = rod_speed;
	node["cursor"] = cursor;

	Json::Value j_rods;

	for(int i = 0; i < size; i++)
	{
		int x = i % width;
		int y = i / width;

		if(rods[i]->get_id() == 0)
		{
			Json::Value j_rod;
			j_rod["id"] = 0;
			j_rods.append(std::move(j_rod));
		}

		else
		{
			Json::Value j_rod(rods[i]->serialize());
			j_rods.append(std::move(j_rod));
		}
	}

	node["rods"] = std::move(j_rods);

	return node;
}

