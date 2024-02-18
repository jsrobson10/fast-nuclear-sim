
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

constexpr float map(float v, float imin, float imax, float omin, float omax)
{
	return (v - imin) * (omax - omin) / (imax - imin) + omin;
}

template<typename A, typename B>
constexpr A mod(A a, B b)
{
	A v = std::fmod(a, b);

	if(v < 0)
	{
		v += b;
	}

	return v;
}

};

