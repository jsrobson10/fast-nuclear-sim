
#pragma once

#include <cmath>
#include <ostream>
#include <glm/matrix.hpp>

namespace Sim::Util::Streams
{

constexpr double show(double v, double m)
{
	v = std::round(v * m) / m;
	return v == 0 ? 0 : v;
}

constexpr double show(double v)
{
	return show(v, 1e3);
}

std::ostream& show_units(std::ostream& o, double v);

};

template <int N, typename T>
std::ostream& operator<<(std::ostream& o, const glm::vec<N, T>& v)
{
	o << "{";

	for(int i = 0; i < N - 1; i++)
	{
		o << v[i] << ", ";
	}

	o << v[N - 1] << "}";
	return o;
}

