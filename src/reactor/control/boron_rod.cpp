
#include "boron_rod.hpp"

#include <cmath>

using namespace sim::reactor::control;

constexpr double boron_density = 2340000; // g/m^3
constexpr double boron_molar_mass = 10; // g/mol
constexpr double boron_molar_density = boron_density / boron_molar_mass; // mol/m^3

BoronRod::BoronRod(const Json::Value& node, coolant::Vessel* v) : coolant::Pipe(node, v)
{
	inserted = node["inserted"].asDouble();
	absorbed = node["absorbed"].asDouble();
}

Json::Value BoronRod::serialize() const
{
	Json::Value node(coolant::Pipe::serialize());
	node["inserted"] = inserted;
	node["absorbed"] = absorbed;
	return node;
}

BoronRod::BoronRod(coolant::Vessel* v) : coolant::Pipe(v)
{

}

void BoronRod::display(std::ostream& o) const
{
	double limit = get_volume() * boron_molar_density;
	
	o << "Inserted: " << (inserted * 100) << "%\n";
	o << "Use: " << (absorbed * limit) << " / " << limit << "\n";
};

void BoronRod::set_reactivity(double a)
{
	inserted = 1 - a;
}

glm::vec4 BoronRod::get_colour() const
{
	double v = inserted * 0.75 + 0.25;
	return {v, v, v, 1};
}

void BoronRod::update(double secs)
{
	double limit = get_volume() * boron_molar_density;

	update_rod(secs);

	double m = (1 - absorbed) * inserted;
	double r_fast = vals[val_t::N_FAST] * m;

	vals[val_t::N_FAST] -= r_fast;
	absorbed += r_fast / limit;

	update_pipe(secs);
}

void BoronRod::update_selected(double a)
{
	inserted -= a;

	if(inserted > 1) inserted = 1;
	if(inserted < 0) inserted = 0;
}

