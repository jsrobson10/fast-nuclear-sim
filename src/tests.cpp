
#include "tests.hpp"
#include "coolant/valve.hpp"
#include "coolant/fluid_holder.hpp"

#include <unistd.h>
#include <iostream>

using namespace sim;
using namespace sim::coolant;

std::ostream& operator<<(std::ostream& o, const fluid_holder& fh)
{
	o << "Fluid Holder\n";
	o << "Heat          " << fh.get_heat() << " C\n";
	o << "Steam         " << fh.get_steam() << " g\n";
	o << "Pressure      " << fh.get_pressure() << " Pa\n";
	o << "Volume        " << fh.get_level() / 1000 << " / " << fh.get_volume() / 1000 << " kL\n\n";

	return o;
}

void tests::run()
{
	fluid_holder src(WATER, 1e6, 0);
	fluid_holder dst(WATER, 1e6, 0);

	src.add_fluid(1e5, 11);

	valve v(&src, &dst, 1, 0.5);
	double dt = 0.001;
	double at = 0;

	for(int i = 0; i < 10000; i++)
	{
		src.update(dt);
		dst.update(dt);
		v.update(dt);

		std::cout << at << "\t" << src.get_pressure() << "\t" << dst.get_pressure() << "\n";
		at += dt;
	}
}

