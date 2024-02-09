
#pragma once

namespace sim::coolant
{

struct vapor_pressure
{
	const double A1, B1, C1;
	const double A2, B2, C2;
	const double T;
	
	constexpr vapor_pressure(double A1, double B1, double C1, double T, double A2, double B2, double C2) :
			A1(A1), B1(B1), C1(C1), T(T), A2(A2), B2(B2), C2(C2) { }

	double calc_p(double t) const;
	double calc_t(double p) const;
};

}

