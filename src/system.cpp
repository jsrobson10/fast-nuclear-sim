
#include "system.hpp"

#include <fstream>

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "graphics/camera.hpp"

using namespace Sim;

Sim::System System::active;

System::System()
{
	const char* layout[] = {
		"      C C C C      ",
		"   C CFCFCFCFC C   ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"CFCFCFCFCFCFCFCFCFC",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		" CFCFCFCFCFCFCFCFC ",
		"  CFCFCFCFCFCFCFC  ",
		"   C CFCFCFCFC C   ",
		"      C C C C      "
	};
	
	vessel = std::make_unique<Reactor::Coolant::Vessel>(Sim::Coolant::WATER, 8, 10, 6e6, 5e5, 10);
	reactor = std::make_unique<Reactor::Reactor>(Sim::Reactor::Builder(19, 19, 1.0 / 4.0, 4, Reactor::Fuel::FuelRod(0.2), vessel.get(), layout));
	condenser = std::make_unique<Coolant::Condenser>(Sim::Coolant::WATER, 6, 4, 3e6, 30000);
	turbine = std::make_unique<Electric::Turbine>(Sim::Coolant::WATER, condenser.get(), 6, 3, 2e6);
	
	sink = std::make_unique<Coolant::Sink>(Sim::Coolant::WATER, 11, 0, 0);
	evaporator = std::make_unique<Coolant::Evaporator>(Sim::Coolant::WATER, 2, 30, 0, 1000);
	condenser_secondary = std::make_unique<Coolant::CondenserSecondary>(condenser.get(), evaporator.get(), 1000);
	
	turbine_inlet_valve = std::make_unique<Coolant::Valve>(vessel.get(), turbine.get(), 0, 0.5);
	turbine_bypass_valve = std::make_unique<Coolant::Valve>(vessel.get(), condenser.get(), 0, 0.5);

	primary_pump = std::make_unique<Coolant::Pump>(condenser.get(), vessel.get(), 1e5, 1, 1e5, 0.1, 10, Coolant::Pump::mode_t::SRC, 35000);
	secondary_pump = std::make_unique<Coolant::Pump>(evaporator.get(), condenser_secondary.get(), 1e5, 1, 1e4, 0.1, 1, Coolant::Pump::mode_t::NONE, 0);
	freight_pump = std::make_unique<Coolant::Pump>(sink.get(), evaporator.get(), 1e5, 1, 1e4, 0.1, 10, Coolant::Pump::mode_t::DST, 1e6);
}

System::System(const Json::Value& node)
{
	clock = node["clock"].asDouble();

	vessel = std::make_unique<Reactor::Coolant::Vessel>(node["vessel"]);
	reactor = std::make_unique<Reactor::Reactor>(node["reactor"], vessel.get());
	condenser = std::make_unique<Coolant::Condenser>(node["condenser"]);
	turbine = std::make_unique<Electric::Turbine>(node["turbine"], condenser.get());

	evaporator = std::make_unique<Coolant::Evaporator>(node["evaporator"]);
	sink = std::make_unique<Coolant::Sink>(evaporator->fluid, 11, 0, 0);
	condenser_secondary = std::make_unique<Coolant::CondenserSecondary>(condenser.get(), evaporator.get(), 1000);

	turbine_inlet_valve = std::make_unique<Coolant::Valve>(node["valve"]["turbine"]["inlet"], vessel.get(), turbine.get());
	turbine_bypass_valve = std::make_unique<Coolant::Valve>(node["valve"]["turbine"]["bypass"], vessel.get(), condenser.get());

	primary_pump = std::make_unique<Coolant::Pump>(node["pump"]["primary"], condenser.get(), vessel.get());
	secondary_pump = std::make_unique<Coolant::Pump>(node["pump"]["secondary"], evaporator.get(), condenser_secondary.get());
	freight_pump = std::make_unique<Coolant::Pump>(node["pump"]["freight"], sink.get(), evaporator.get());
}

void System::update(double dt)
{
	dt *= speed;
	clock += dt;
	
	turbine_inlet_valve->update(dt);
	turbine_bypass_valve->update(dt);
	primary_pump->update(dt);
	secondary_pump->update(dt);
	freight_pump->update(dt);
	reactor->update(dt);

	vessel->update(dt);
	turbine->update(dt);
	condenser->update(dt);
	evaporator->update(dt);
	condenser_secondary->update(dt);
}

System::operator Json::Value() const
{
	Json::Value node;

	node["vessel"] = *vessel;
	node["turbine"] = *turbine;
	node["condenser"] = *condenser;
	node["evaporator"] = *evaporator;
	node["pump"]["primary"] = *primary_pump;
	node["pump"]["secondary"] = *secondary_pump;
	node["pump"]["freight"] = *freight_pump;
	node["valve"]["turbine"]["inlet"] = *turbine_inlet_valve;
	node["valve"]["turbine"]["bypass"] = *turbine_bypass_valve;
	node["reactor"] = *reactor;
	node["clock"] = clock;

	return node;
}

void System::save()
{
	Json::Value root(active);
	root["camera"] = Graphics::Camera::serialize();

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream savefile("savefile.json");
	writer->write(root, &savefile);
	savefile.close();
}

void System::load()
{
	Json::Value root;
	std::ifstream savefile("savefile.json");
	savefile >> root;
	savefile.close();

	System sys(root);
	Graphics::Camera::load(root["camera"]);
	active = std::move(sys);
}

