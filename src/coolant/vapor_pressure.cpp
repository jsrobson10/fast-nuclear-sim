
#include "vapor_pressure.hpp"

#include <cmath>

using namespace sim::coolant;

double vapor_pressure::calc_p(double t) const
{
	return t > -C ? std::pow(10, A - B / (C + t)) : 0;
}

double vapor_pressure::calc_t(double p) const
{
	return B / (A - std::log(p) / std::log(10)) - C;
}

