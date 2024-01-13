
#include "sample.hpp"
#include "half_life.hpp"

using namespace sim;

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

	// simulate waste and extract products
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
	double neutrons = (slow_neutrons + NEUTRON_BG * secs) * (fuel / volume);
	slow_neutrons = 0;

	// deal with this edge case
	if(neutrons > fuel)
	{
		slow_neutrons = neutrons - fuel;
		neutrons = fuel;
	}

	// simulate fuel use
	fuel -= neutrons;
	energy += neutrons;
	fast_neutrons += neutrons * 3;
	waste.add_fissile(neutrons * 2);

	// add the poison
	te_135 += neutrons;
}

double sample::get_volume() const
{
	return mass + xe_135 * Xe_135_M;
}

double sample::get_mass() const
{
	return mass;
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

