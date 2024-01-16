
#include "vessel.hpp"
#include "../../constants.hpp"
#include "../../conversions/temperature.hpp"

#include <cmath>

using namespace sim::reactor::coolant;

vessel::vessel(double level, double volume, sim::coolant::fluid_t fluid) : volume(volume), fluid(fluid)
{
	this->level = level;
}

void vessel::update()
{
	double vp = fluid.vapor_pressure.calc_p(heat);
	double sc = vp * (volume - level) / (constants::R * conversions::temperature::c_to_k(heat)) - steam;
	double sc_l = fluid.mol_to_l(sc);

	if(sc_l > level) sc_l = level;

	steam += fluid.l_to_mol(sc_l);
	level -= sc_l;
}

double vessel::add_heat(double amount)
{
	double th = get_heat();

	if(amount > th)
	{
		amount -= th;
		heat += amount / level;

		return th;
	}

	return amount;
}

double vessel::get_pressure() const
{
	return (steam * conversions::temperature::c_to_k(heat) * constants::R) / (volume - level);
}

std::ostream& operator<<(std::ostream& o, const vessel& v)
{
	o << "Volume: " << v.get_volume() << " L\n";
	o << "Level: " << v.get_level() << " L\n";
	o << "Steam: " << v.get_steam() << " mol\n";
	o << "Heat: " << v.get_heat() << " C\n";
	o << "Pressure: " << v.get_pressure() << " Pa\n";
	o << "Vapor Pressure: " << v.fluid.vapor_pressure.calc_p(v.get_heat()) << " Pa\n";

	return o;
}


