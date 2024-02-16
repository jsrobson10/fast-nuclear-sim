
#include "vapor_pressure.hpp"

#include <cmath>

using namespace Sim::Coolant;

double VaporPressure::calc_p(double t) const
{
	return t > -C ? std::pow(10, A - B / (C + t)) : 0;
}

double VaporPressure::calc_t(double p) const
{
	return B / (A - std::log(p) / std::log(10)) - C;
}

VaporPressure::VaporPressure(const Json::Value& node) :
		A(node["A"].asDouble()),
		B(node["B"].asDouble()),
		C(node["C"].asDouble())
{

}

VaporPressure::operator Json::Value() const
{
	Json::Value node;

	node["A"] = A;
	node["B"] = B;
	node["C"] = C;

	return node;
}

