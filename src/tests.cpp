
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
	fluid_holder fhs[] = {
		fluid_holder(WATER, 75398, 0),
	};

	valve vs[] = {
	};

	fhs[0].level = 100;
	fhs[0].steam = 0;
	fhs[0].heat = 100;

	double dt = 0.1;
	double at = 0;

	std::cout << "time";

	for(fluid_holder& fh : fhs)
	{
		std::cout << "\t\tlevel (L)\tsteam (g)\theat (C)\tpressure (Pa)";
	}

	std::cout << "\n";

	for(int i = 0; i < 10000; i++)
	{
		for(fluid_holder& fh : fhs)
		{
			fh.update(dt);
		}
		
		for(valve& v : vs)
		{
			v.update(dt);
		}

		std::cout << at;

		for(const fluid_holder& fh : fhs)
		{
			std::cout << "\t\t" << fh.get_level() << "\t" << fh.get_steam() << "\t" << fh.get_heat() << "\t" << fh.get_pressure();
		}
		
		std::cout << "\n";
		at += dt;
	}
	
	std::cout << "\n" << fhs[0] << "\n";

}

