
#include "sample.hpp"
#include "half_life.hpp"

#include <iostream>

using namespace sim::reactor::fuel;

constexpr double NEUTRON_BG = 1e-30;

sample::sample(double fuel)
{
	this->fuel = fuel;
	this->u_238 = 1 - fuel;
	this->mass = 1;
}

void sample::update(double secs)
{
	double m;
	
	// decay waste and extract products
	waste.update(secs);
	fast_neutrons += waste.extract_neutrons();
	energy += waste.extract_energy() * (1.0 / 30.0) / secs;

	// decay Xe-135
	xe_135 *= half_life::get(secs, half_life::Xe_135);

	// decay I-135 into Xe-135
	m = half_life::get(secs, half_life::I_135);
	xe_135 += i_135 * (1 - m);
	i_135 *= m;

	// decay Te-135 into I-135
	m = half_life::get(secs, half_life::Te_135);
	i_135 += te_135 * (1 - m);
	te_135 *= m;

	// absorb neutrons
	double volume = get_volume();
	double neutrons = slow_neutrons + NEUTRON_BG * secs;
	double neutrons_fuel = neutrons * (fuel / volume);
	double neutrons_iodine = neutrons * (i_135 / volume);
	double neutrons_xenon = neutrons * ((xe_135 * Xe_135_M) / volume);
	double neutrons_total = neutrons;
	slow_neutrons = 0;

	te_135 += neutrons_fuel * (1.0 / 50.0);
	xe_135 -= neutrons_xenon;
	i_135 -= neutrons_iodine;
	fuel -= neutrons_fuel;
	
	// deal with these edge cases
	if(xe_135 < 0) { slow_neutrons += xe_135; xe_135 = 0; }
	if(i_135 < 0) { slow_neutrons += i_135; i_135 = 0; }
	if(fuel < 0) { slow_neutrons += fuel; fuel = 0; }

	efficiency = neutrons_fuel / neutrons_total;
	
	// simulate fuel use
	energy += neutrons_fuel / secs * 0.8;
	waste.add_fissile(neutrons_fuel * 6);
}

void sample::display(std::ostream& o) const
{
	o << "Fuel: " << fuel << "\n";
	o << "Mass: " << mass << "\n";
	o << "Energy: " << energy << "\n";
	o << "Neutrons:\n  Fast: " << fast_neutrons << "\n  Slow: " << slow_neutrons << "\n";
	o << "Poison:\n  Te-135: " << te_135 << "\n  I-135: " << i_135 << "\n  Xe-135: " << xe_135 << "\n";
	o << "Neutron Absorbtion: " << (fuel / get_volume()) << "\n";
}

