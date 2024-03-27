
#include "loop.hpp"

using namespace Sim::Coolant;

//Pump::Pump(FluidHolder* src, FluidHolder* dst, double mass, double radius, double power, double l_per_rev, double friction, mode_t mode, double target) :

Loop::Loop(Reactor::Coolant::Vessel* vessel, Evaporator* evaporator, Electric::Grid* grid) :
		condenser(WATER, 6, 4, 3e6, 30000),
		turbine(&condenser),
		generator(&turbine, grid, 6, 3, 2e6),
		condenser_secondary(&condenser, evaporator),
		turbine_inlet_valve(vessel, &turbine, 0, 0.5),
		turbine_bypass_valve(vessel, &condenser, 0, 0.5),
		primary_pump(&condenser, vessel, 1e5, 1, 1e6, 0.1, 10000, Pump::mode_t::SRC, 35000),
		secondary_pump(evaporator, &condenser_secondary, 1e5, 1, 1e6, 0.1, 10000, Pump::mode_t::NONE, 0)
{

}

Loop::Loop(const Json::Value& node, Reactor::Coolant::Vessel* vessel, Evaporator* evaporator, Electric::Grid* grid) :
		condenser(node["condenser"]),
		turbine(&condenser),
		generator(node["generator"], &turbine, grid),
		condenser_secondary(&condenser, evaporator),
		turbine_inlet_valve(node["valve"]["inlet"], vessel, &turbine),
		turbine_bypass_valve(node["valve"]["bypass"], vessel, &condenser),
		primary_pump(node["pump"]["primary"], &condenser, vessel),
		secondary_pump(node["pump"]["secondary"], evaporator, &condenser_secondary)
{

}

void Loop::update(double dt)
{
	turbine_inlet_valve.update(dt);
	turbine_bypass_valve.update(dt);
	condenser.update(dt);

	turbine.update(dt);
	generator.update(dt);

	primary_pump.update(dt);
	secondary_pump.update(dt);
	condenser_secondary.update(dt);
}

Loop::operator Json::Value() const
{
	Json::Value node;
	
	node["generator"] = generator;
	node["condenser"] = condenser;
	node["pump"]["primary"] = primary_pump;
	node["pump"]["secondary"] = secondary_pump;
	node["valve"]["inlet"] = turbine_inlet_valve;
	node["valve"]["bypass"] = turbine_bypass_valve;

	return node;
}

