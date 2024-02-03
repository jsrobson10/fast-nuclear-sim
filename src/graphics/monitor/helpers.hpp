
#pragma once

#include <cmath>

constexpr double show(double v, double m)
{
	return std::round(v * m) / m;
}

constexpr double show(double v)
{
	return std::round(v * 1e3) * 1e-3;
}
