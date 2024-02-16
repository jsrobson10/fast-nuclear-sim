
#pragma once

#include <json/json.h>

namespace Sim::Coolant
{

struct VaporPressure
{
	const double A, B, C;
	
	constexpr VaporPressure(double A, double B, double C) : A(A), B(B), C(C) { }

	VaporPressure(const Json::Value& node);
	operator Json::Value() const;

	double calc_p(double t) const;
	double calc_t(double p) const;
};

}

