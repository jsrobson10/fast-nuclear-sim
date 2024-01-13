
#pragma once

#include <cmath>

namespace sim::half_life
{

const double Te_135 = 19;
const double I_135 = 23652;
const double Xe_135 = 32904;
const double Cs_135 = 41971608000000;

constexpr double get(double secs, double hl) noexcept
{
	return std::pow(0.5, secs / hl);
}

};

