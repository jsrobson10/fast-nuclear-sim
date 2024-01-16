
#include "vapor_pressure.hpp"
#include "../conversions/temperature.hpp"
#include "../conversions/pressure.hpp"

#include <cmath>

using namespace sim::coolant;
using namespace sim::conversions;

double vapor_pressure::calc_p(double t) const
{
	return pressure::torr_to_pa(std::pow(10, A - B / ( C + t ) ));
}

double vapor_pressure::calc_t(double p) const
{
	return B / ( A - std::log(pressure::pa_to_torr(p)) / std::log(10) ) - C;
}

