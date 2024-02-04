
#pragma once

#include <cmath>

constexpr double show(double v, double m)
{
	return std::round(v * m) / m;
}

constexpr double show(double a)
{
	double b = std::round(a * 1e3) * 1e-3;
	return b == 0 ? 0 : b;
}
