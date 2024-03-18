
#pragma once

#include <ostream>
#include <cmath>

namespace Sim::Util::Math
{

constexpr double ms2_to_j(double v, double mass)
{
	double m = (v < 0) ? -1 : 1;
	return m*std::pow(m * v / (mass * 0.001), 2);
}

constexpr double j_to_ms2(double j, double mass)
{
	double m = (j < 0) ? -1 : 1;
	return m*std::sqrt(m * j / (mass * 0.001));
}

template <typename A>
constexpr A map(A v, auto imin, auto imax, auto omin, auto omax)
{
	return (v - imin) * (omax - omin) / (imax - imin) + omin;
}

template <typename A>
constexpr A clamp(A v, auto min, auto max)
{
	if(v < min)
		return min;
	if(v > max)
		return max;
	return v;
}

template <typename A>
constexpr A mod(A a, auto b)
{
	A v = std::fmod(a, b);

	if(v < 0)
	{
		v += b;
	}

	return v;
}

};

