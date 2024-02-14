
#pragma once

#include <glm/matrix.hpp>
#include <ostream>
#include <cmath>

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

namespace sim::util
{

constexpr double calc_work(double j, double mass)
{
	double m = 1;

	if(j < 0)
	{
		m = -1;
	}

	return m * std::sqrt(m * j / (mass * 0.001));
}

};

