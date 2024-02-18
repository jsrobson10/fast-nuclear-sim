
#include "turbine.hpp"
#include "../system.hpp"
#include "../util/math.hpp"
#include "../util/streams.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Electric;

constexpr static double calc_cylinder(double h, double d)
{
	double r = d / 2;
	
	return M_PI * r * r * h * 1000;
}

Turbine::Turbine(Coolant::Fluid type, Coolant::Condenser* condenser, double length, double diameter, double mass) :
		length(length), diameter(diameter), condenser(condenser),
		Sim::Coolant::FluidHolder(type, calc_cylinder(length, diameter), mass)
{
	
}

Turbine::Turbine(const Json::Value& node, Coolant::Condenser* condenser) :
		condenser(condenser),
		length(node["length"].asDouble()),
		diameter(node["diameter"].asDouble()),
		friction(node["friction"].asDouble()),
		Sim::Coolant::FluidHolder(node)
{
	velocity = node["velocity"].asDouble();
	phase = node["phase"].asDouble();
	breaker_closed = node["breaker_closed"].asBool();
	energy_input = node["energy_input"].asDouble();
	energy_generated = node["energy_generated"].asDouble();
}

void Turbine::update(double dt)
{
	double energy_friction = get_rpm() / 60 * dt * friction;
	double work = Util::Math::j_to_ms2(energy_input - energy_friction, extra_mass);
	phase = std::fmod(phase + Util::Math::map( get_rpm(), 0, 60, 0, 2 * M_PI ) * dt, 2 * M_PI);

	// do energy transfer stuff here
	if(breaker_closed)
	{
		double a = get_phase_diff();
		double dist_extra = 0.1;
	
		if(is_stable || (a < 1e-5 && std::abs(get_rpm() - 3600) < 1e-3))
		{
			is_stable = true;
			energy_generated = (energy_input - energy_friction) / dt;
			energy_input = 0;
			phase -= a;
			set_rpm(3600);
			return;
		}
	
		glm::vec<2, double> point(std::cos(a), std::sin(a));
		glm::vec<2, double> diff1 = point - glm::vec<2, double>(dist_extra + 1, 0);
		glm::vec<2, double> diff2 = point - glm::vec<2, double>(-dist_extra - 1, 0);

		double strength = 1e10;
		double dist1 = glm::length(diff1);
		double dist2 = glm::length(diff2);
		double force1_len = -strength / (4 * M_PI * dist1 * dist1);
		double force2_len = strength / (4 * M_PI * dist2 * dist2);
		glm::vec<2, double> force1 = diff1 / dist1 * force1_len;
		glm::vec<2, double> force2 = diff2 / dist2 * force2_len;

		// calc the projected force vector

		double t1 = std::tan(a);
		double t2 = std::tan(a + M_PI/2);

		glm::vec<2, double> proj1, proj2;
		proj1.x = ((point.x + force1.x) * t1 - point.x * t2 - force1.y) / (t1 - t2);
		proj2.x = ((point.x + force2.x) * t1 - point.x * t2 - force2.y) / (t1 - t2);
		proj1.y = (proj1.x - point.x) * t2 + point.y;
		proj2.y = (proj2.x - point.x) * t2 + point.y;

		glm::mat<2, 2, double> rot_mat = {
			point.x, -point.y,
			point.y, point.x,
		};

		double eddy = (get_rpm() - 3600) * dt * 1e5;

		// calc the amount of actual work (in change in m/s) done

		glm::vec<2, double> proj = rot_mat * (proj1 + proj2) * 0.5;
		double work_done = proj.y / (extra_mass * 0.001) * dt - eddy;

		work += work_done;
		energy_generated = -Util::Math::ms2_to_j(work_done / dt, extra_mass) / dt;
	}

	else
	{
		energy_generated = 0;
		is_stable = false;
	}

	velocity = std::max(velocity + work, 0.0);
	energy_input = 0;
}

double Turbine::get_rpm() const
{
	return velocity / (M_PI * extra_mass * 0.001 * diameter * diameter * 0.25);
}

void Turbine::set_rpm(double rpm)
{
	velocity = rpm * (M_PI * extra_mass * 0.001 * diameter * diameter * 0.25);
}

double Turbine::get_phase_diff() const
{
	double phase_g = System::active.clock * 120 * M_PI;
	return Util::Math::mod(phase - phase_g + M_PI, 2*M_PI) - M_PI;
}

void Turbine::add_gas(double steam, double air, double t)
{
	energy_input += (steam + air) * fluid.jPg; // J
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
	node["energy_input"] = energy_input;
	node["energy_generated"] = energy_generated;
	node["phase"] = phase;
	
	return node;
}

