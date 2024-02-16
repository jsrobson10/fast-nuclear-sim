
#include "turbine.hpp"
#include "../system.hpp"
#include "../util/math.hpp"

#include <cmath>
#include <iostream>

using namespace sim::electric;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

Turbine::Turbine(coolant::Fluid type, coolant::Condenser* condenser, double length, double diameter, double mass) :
		length(length), diameter(diameter), condenser(condenser),
		sim::coolant::FluidHolder(type, calc_cylinder(length, diameter), mass)
{
	
}

Turbine::Turbine(const Json::Value& node, coolant::Condenser* condenser) :
		condenser(condenser),
		length(node["length"].asDouble()),
		diameter(node["diameter"].asDouble()),
		friction(node["friction"].asDouble()),
		sim::coolant::FluidHolder(node)
{
	velocity = node["velocity"].asDouble();
	phase = node["phase"].asDouble();
	breaker_closed = node["breaker_closed"].asBool();
}

void Turbine::update(double dt)
{
	double work = get_rpm() / 60 * dt * friction;
	phase = std::fmod(phase + util::map( get_rpm(), 0, 60, 0, 2 * M_PI ) * dt, 2 * M_PI);

	// do energy transfer stuff here
	if(breaker_closed)
	{
		double r_diff = util::map(get_phase_diff(), -M_PI, M_PI, -30, 30);
		double w = r_diff * 1e6;

		double v2 = util::mod((velocity - 3600) / 60 + 30, 60) - 30;
		double w2 = w * w * v2;

		energy_generated = w2 * extra_mass;
		work += w * dt;
	}

	else
	{
		energy_generated = 0;
	}

	velocity = std::max(velocity - work, 0.0);

}

double Turbine::get_rpm() const
{
	return velocity / (M_PI * extra_mass * 0.001 * diameter * diameter * 0.25);
}

double Turbine::get_phase_diff() const
{
	double phase_g = std::fmod(System::active.clock * 60, 1) * 2 * M_PI;
	return util::mod(phase - phase_g + M_PI, 2*M_PI) - M_PI;
}

void Turbine::add_gas(double steam, double air, double t)
{
	double joules = (steam + air) * fluid.jPg;
	velocity = std::max(velocity + util::calc_work(joules, extra_mass), 0.0);
	condenser->add_gas(steam, air, t);
}

Turbine::operator Json::Value() const
{
	Json::Value node(FluidHolder::operator::Json::Value());

	node["length"] = length;
	node["diameter"] = diameter;
	node["velocity"] = velocity;
	node["friction"] = friction;
	node["breaker_closed"] = breaker_closed;
	node["phase"] = phase;
	
	return node;
}

