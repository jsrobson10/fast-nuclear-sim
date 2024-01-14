
#include "sample.hpp"
#include "half_life.hpp"

using namespace sim::fuel;

static const double Xe_135_M = 1e4;
static const double NEUTRON_BG = 1e-10;

sample::sample(double fuel, double mass)
{
	this->fuel = fuel;
	this->mass = mass;
}

void sample::update(double secs)
{
	double m;

	// sim::fuelulate waste and extract products
	waste.update(secs);
	fast_neutrons += waste.extract_neutrons();
	energy += waste.extract_energy();

	// decay Xe-135
	m = half_life::get(secs, half_life::Xe_135);
	xe_135 *= m;

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
	slow_neutrons = 0;

	// deal with these edge cases
	if(neutrons_fuel > fuel) neutrons_fuel = fuel;
	if(neutrons_xenon > xe_135) neutrons_xenon = xe_135;
	if(neutrons_iodine > i_135) neutrons_iodine = i_135;

	// sim::fuelulate fuel use
	fuel -= neutrons_fuel;
	energy += neutrons_fuel;
	fast_neutrons += neutrons_fuel * 3;
	waste.add_fissile(neutrons_fuel * 2);

	// do the poison
	te_135 += neutrons_fuel * (1.0 / 8.0);
	xe_135 -= neutrons_xenon;
	i_135 -= neutrons_iodine;
}

double sample::get_volume() const
{
	return mass + xe_135 * Xe_135_M;
}

double sample::extract_energy()
{
	double v = energy;
	energy = 0;
	return v;
}

double sample::extract_fast_neutrons()
{
	double v = fast_neutrons;
	fast_neutrons = 0;
	return v;
}

void sample::add_slow_neutrons(double a)
{
	slow_neutrons += a;
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

