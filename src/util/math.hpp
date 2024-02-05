
#pragma once

#include <glm/matrix.hpp>
#include <ostream>

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

