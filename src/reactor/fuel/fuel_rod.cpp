
#include "fuel_rod.hpp"

#include <cmath>

using namespace Sim::Reactor::Fuel;

constexpr double fuel_density = 19100000; // g/m^3
constexpr double fuel_molar_mass = 238.029; // g/mol
constexpr double fuel_molar_density = fuel_density / fuel_molar_mass; // mol/m^3

FuelRod::FuelRod(double fuel) : s(fuel)
{

}

FuelRod::FuelRod(const Json::Value& node) : s(node["sample"]), Rod(node)
{
	
}

Json::Value FuelRod::serialize() const
{
	Json::Value node(Rod::serialize());
	node["sample"] = s;
	return node;
}

void FuelRod::display(std::ostream& o) const
{
	double mol = fuel_molar_density * get_volume() * 0.001;
	
	o << "Fuel: " << (s.get_fuel() * mol) << " / " << (s.get_mass() * mol) << " mol\n";
	o << "Efficiency: " << (s.get_efficiency() * 100) << " %\n";
	o << "Output: " << get_energy_output() << " W\n";
	o << "Iodine: " << (s.get_i_135() * mol) << " mol\n";
	o << "Xenon: " << (s.get_xe_135() * mol) << " mol\n";
}

double FuelRod::get_energy_output() const
{
	double mol = fuel_molar_density * get_volume() * 0.001;

	// mol * J/mol/s

	return s.get_energy() * mol;
}

void FuelRod::update(double secs)
{
	update_rod(secs);
	
	double mol = fuel_molar_density * get_volume() * 0.001;
	
	s.clear_energy();
	s.clear_fast_neutrons();
	s.clear_slow_neutrons();
	s.add_slow_neutrons(vals[val_t::N_SLOW] / mol);
	s.update(secs);

	vals[val_t::HEAT] += s.get_energy() * mol * secs;
	vals[val_t::N_FAST] += s.get_fast_neutrons() * mol;
	vals[val_t::N_SLOW] = s.get_slow_neutrons() * mol;
}

