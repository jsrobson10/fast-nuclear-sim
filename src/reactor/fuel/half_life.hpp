
#pragma once

#include <cmath>

namespace sim::reactor::fuel::half_life
{

const double Te_135 = 19;
const double I_135 = 23652;
const double Xe_135 = 32904;
const double Cs_135 = 41971608000000;
const double U_235 = 2.22e16;
const double U_238 = 1.41e17;

template <typename T>
constexpr T get(T secs, T hl) noexcept
{
	return std::pow(0.5, secs / hl);
}


};

