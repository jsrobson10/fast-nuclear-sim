
#pragma once

#include <json/json.h>

namespace sim::coolant
{

struct vapor_pressure
{
	const double A, B, C;
	
	constexpr vapor_pressure(double A, double B, double C) : A(A), B(B), C(C) { }

	vapor_pressure(const Json::Value& node);
	operator Json::Value() const;

	double calc_p(double t) const;
	double calc_t(double p) const;
};

}

