
#include "streams.hpp"

using namespace Sim::Util;

std::ostream& Streams::show_units(std::ostream& o, double v)
{
	const char* PREFIXES[] = {
		" ",
		" k",
		" M",
		" G",
		" T",
		" P",
		" E",
		" Z",
		" Y",
		" R",
		" Q"
	};
	
	int unit_id = 0;

	while(std::abs(v) > 10000)
	{
		unit_id++;
		v *= 0.001;
	}

	if(unit_id > 10)
	{
		unit_id = 10;
	}

	o << show(v) << PREFIXES[unit_id];
	return o;
}

