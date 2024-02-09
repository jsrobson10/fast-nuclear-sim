
#include "vapor_pressure.hpp"
#include "../conversions/temperature.hpp"
#include "../conversions/pressure.hpp"

#include <cmath>

using namespace sim::coolant;
using namespace sim::conversions;

double vapor_pressure::calc_p(double t) const
{
	double p;
   
	if(t < T)
	{
		p = std::pow(10, A1 - B1 / ( C1 + t ) );
	}

	else
	{
		p = std::pow(10, A2 - B2 / ( C2 + t ) );
	}

	return pressure::mmhg_to_pa(p);
}

double vapor_pressure::calc_t(double p) const
{
	double P = pressure::pa_to_mmhg(p);

	if(p < calc_p(T))
	{
		return B1 / ( A1 - std::log(P) / std::log(10) ) - C1;
	}

	else
	{
		return B2 / ( A2 - std::log(P) / std::log(10) ) - C2;
	}
}

