
#include "generator.hpp"
#include "../util/math.hpp"
#include "../util/streams.hpp"

#include <cmath>
#include <iostream>

using namespace Sim::Electric;

Generator::Generator(Turbine* turbine, Grid* grid, double length, double diameter, double mass) :
		length(length), diameter(diameter), mass(mass), turbine(turbine), grid(grid)
{
	
}

Generator::Generator(const Json::Value& node, Turbine* turbine, Grid* grid) :
		length(node["length"].asDouble()),
		diameter(node["diameter"].asDouble()),
		friction(node["friction"].asDouble()),
		mass(node["mass"].asDouble()),
		turbine(turbine),
		grid(grid)
{
	velocity = node["velocity"].asDouble();
	phase = node["phase"].asDouble();
	breaker_closed = node["breaker_closed"].asBool();
	energy_generated = node["energy_generated"].asDouble();
}

void Generator::update(double dt)
{
	double energy_input = turbine->extract_energy();
	double energy_friction = get_rpm() / 60 * dt * friction;
	double work = Util::Math::j_to_ms2(energy_input - energy_friction, mass);
	phase = std::fmod(phase + Util::Math::map( get_rpm(), 0, 3600, 0, 120 * M_PI ) * dt, 4 * M_PI);

	// do energy transfer stuff here
	if(breaker_closed)
	{
		double a = get_phase_diff();
		double dist_extra = 0.1;
	
		if(is_stable || (a < 1e-5 && std::abs(get_rpm() - 60 * grid->frequency) < 1e-3))
		{
			is_stable = true;
			energy_generated = (energy_input - energy_friction) / dt;
			grid->pull_energy(energy_friction - energy_input);
			phase -= a;
			set_rpm(grid->frequency * 60);
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

		double eddy = (get_rpm() - 60 * grid->frequency) * dt * 1e5;

		// calc the amount of actual work (in change in m/s) done

		glm::vec<2, double> proj = rot_mat * (proj1 + proj2) * 0.5;
		double work_done = proj.y / (mass * 0.001) * dt - eddy;

		work += work_done;
		double e = Util::Math::ms2_to_j(work_done / dt, mass);
		energy_generated = -e / dt;
		grid->pull_energy(e);

		if(grid->is_above_limit())
		{
			breaker_closed = false;
		}
	}

	else
	{
		energy_generated = 0;
		is_stable = false;
	}

	velocity = std::max(velocity + work, 0.0);
}

double Generator::get_rpm() const
{
	return velocity / (M_PI * mass * 0.001 * diameter * diameter * 0.25);
}

void Generator::set_rpm(double rpm)
{
	velocity = rpm * (M_PI * mass * 0.001 * diameter * diameter * 0.25);
}

double Generator::get_phase_diff() const
{
	return Util::Math::mod(phase - grid->get_phase() + M_PI, 2*M_PI) - M_PI;
}

double Generator::get_frequency() const
{
	return get_rpm() / 60;
}

double Generator::get_power() const
{
	return energy_generated;
}

double Generator::get_voltage() const
{
	// TODO: implement this
	return get_frequency() / 60 * 20e3;
}

Generator::operator Json::Value() const
{
	Json::Value node;

	node["length"] = length;
	node["diameter"] = diameter;
	node["velocity"] = velocity;
	node["friction"] = friction;
	node["breaker_closed"] = breaker_closed;
	node["energy_generated"] = energy_generated;
	node["phase"] = phase;
	node["mass"] = mass;
	
	return node;
}

