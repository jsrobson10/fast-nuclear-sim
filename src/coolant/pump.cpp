
#include "pump.hpp"
#include "../util/math.hpp"

#include <cmath>
#include <iostream>
#include <string>

using namespace Sim::Coolant;

Pump::Pump(FluidHolder* src, FluidHolder* dst, double mass, double radius, double power, double l_per_rev, double friction, mode_t mode, double target) :
		src(src),
		dst(dst),
		mass(mass),
		radius(radius),
		l_per_rev(l_per_rev),
		friction(friction),
		max_power(power),
		mode(mode),
		target(target)
{
	
}

double Pump::get_flow_target() const
{
	return l_per_rev * get_rpm() * 60;
}

double Pump::get_flow() const
{
	return flow;
}

double Pump::get_flow_mass() const
{
	return src->fluid.l_to_g(flow);
}

double Pump::get_rpm() const
{
	return velocity / (M_PI * mass * 0.001 * radius * radius);
}

double Pump::get_power() const
{
	return power;
}

const char* Pump::get_state_string()
{
	if(!powered) return "Off";
	if(power == 0) return "Idle";
	return "On";
}

void Pump::update(double dt)
{
	if(powered)
	{
		switch(mode)
		{
		case mode_t::SRC:
			power = pid.calculate(dt, target, src->get_gas_volume());
			break;
		case mode_t::DST:
			power = pid.calculate(dt, target, dst->get_level());
			break;
		case mode_t::NONE:
			power = 1;
			break;
		}

		velocity += Util::Math::j_to_ms2(dt * power * max_power, mass);
	}

	else
	{
		power = 0;
	}
	
	double src_heat = src->get_heat();
	double p_diff_1 = dst->get_pressure() - src->get_pressure();

	double max_volume = std::min(src->get_level(), dst->get_volume() - dst->get_level());
	double src_volume = src->extract_fluid(std::min(get_flow_target() * dt, max_volume));
	double dst_volume = dst->add_fluid(src_volume, src_heat);

	double p_diff_2 = dst->get_pressure() - src->get_pressure();
	double p_diff = (p_diff_1 + p_diff_2) / 2;
	double work = p_diff * dst_volume * 0.001 + get_rpm() / 60 * dt * friction;

	velocity = std::max(velocity - Util::Math::j_to_ms2(work, mass), 0.0);
	flow = dst_volume / dt;
}

static Pump::mode_t get_mode(std::string mode)
{
	if(mode == "SRC")
		return Pump::mode_t::SRC;
	if(mode == "DST")
		return Pump::mode_t::DST;

	return Pump::mode_t::NONE;
}

Pump::Pump(const Json::Value& node, FluidHolder* src, FluidHolder* dst) :
		mode(get_mode(node["mode"].asString())),
		mass(node["mass"].asDouble()),
		radius(node["radius"].asDouble()),
		l_per_rev(node["l_per_rev"].asDouble()),
		friction(node["friction"].asDouble()),
		max_power(node["max_power"].asDouble()),
		target(node["target"].asDouble()),
		pid(node["pid"]),
		src(src),
		dst(dst)
{
	flow = node["flow"].asDouble();
	velocity = node["velocity"].asDouble();
	power = node["power"].asDouble();
	powered = node["powered"].asBool();
}

Pump::operator Json::Value() const
{
	Json::Value node;

	node["pid"] = pid;
	node["flow"] = flow;
	node["velocity"] = velocity;
	node["power"] = power;
	node["mass"] = mass;
	node["radius"] = radius;
	node["l_per_rev"] = l_per_rev;
	node["friction"] = friction;
	node["max_power"] = max_power;
	node["target"] = target;
	node["powered"] = powered;

	switch(mode)
	{
	case mode_t::SRC:
		node["mode"] = "SRC";
		break;
	case mode_t::DST:
		node["mode"] = "DST";
		break;
	case mode_t::NONE:
		node["mode"] = "NONE";
		break;
	}

	return node;
}

