
#include "system.hpp"

#include <fstream>

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "graphics/camera.hpp"

using namespace sim;

sim::system system::active;

system::system()
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
	
	vessel = std::make_unique<reactor::coolant::vessel>(sim::coolant::WATER, 8, 10, 6e6, 5e5, 10);
	reactor = std::make_unique<reactor::reactor>(sim::reactor::builder(19, 19, 1.0 / 4.0, 4, reactor::fuel::fuel_rod(0.2), vessel.get(), layout));
	condenser = std::make_unique<coolant::condenser>(sim::coolant::WATER, 6, 4, 3e6, 30000);
	turbine = std::make_unique<electric::turbine>(sim::coolant::WATER, condenser.get(), 6, 3, 2e6);
	
	sink = std::make_unique<coolant::sink>(sim::coolant::WATER, 11, 0, 0);
	evaporator = std::make_unique<coolant::evaporator>(sim::coolant::WATER, 2, 30, 0, 1000);
	condenser_secondary = std::make_unique<coolant::condenser_secondary>(condenser.get(), evaporator.get(), 1000);
	
	turbine_inlet_valve = std::make_unique<coolant::valve>(vessel.get(), turbine.get(), 0, 0.5);
	turbine_bypass_valve = std::make_unique<coolant::valve>(vessel.get(), condenser.get(), 0, 0.5);

	primary_pump = std::make_unique<coolant::pump>(condenser.get(), vessel.get(), 1e5, 1, 1e5, 0.1, 10, coolant::pump::mode_t::SRC, 35000);
	secondary_pump = std::make_unique<coolant::pump>(evaporator.get(), condenser_secondary.get(), 1e5, 1, 1e4, 0.1, 1, coolant::pump::mode_t::NONE, 0);
	freight_pump = std::make_unique<coolant::pump>(sink.get(), evaporator.get(), 1e5, 1, 1e4, 0.1, 10, coolant::pump::mode_t::DST, 1e6);
}

system::system(const Json::Value& node)
{
	clock = node["clock"].asDouble();

	vessel = std::make_unique<reactor::coolant::vessel>(node["vessel"]);
	reactor = std::make_unique<reactor::reactor>(node["reactor"], vessel.get());
	condenser = std::make_unique<coolant::condenser>(node["condenser"]);
	turbine = std::make_unique<electric::turbine>(node["turbine"], condenser.get());

	evaporator = std::make_unique<coolant::evaporator>(node["evaporator"]);
	sink = std::make_unique<coolant::sink>(evaporator->fluid, 11, 0, 0);
	condenser_secondary = std::make_unique<coolant::condenser_secondary>(condenser.get(), evaporator.get(), 1000);

	turbine_inlet_valve = std::make_unique<coolant::valve>(node["valve"]["turbine"]["inlet"], vessel.get(), turbine.get());
	turbine_bypass_valve = std::make_unique<coolant::valve>(node["valve"]["turbine"]["bypass"], vessel.get(), condenser.get());

	primary_pump = std::make_unique<coolant::pump>(node["pump"]["primary"], condenser.get(), vessel.get());
	secondary_pump = std::make_unique<coolant::pump>(node["pump"]["secondary"], evaporator.get(), condenser_secondary.get());
	freight_pump = std::make_unique<coolant::pump>(node["pump"]["freight"], sink.get(), evaporator.get());
}

void system::update(double dt)
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

system::operator Json::Value() const
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

void system::save()
{
	Json::Value root(active);
	root["camera"] = graphics::camera::serialize();

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream savefile("savefile.json");
	writer->write(root, &savefile);
	savefile.close();
}

void system::load()
{
	Json::Value root;
	std::ifstream savefile("savefile.json");
	savefile >> root;
	savefile.close();

	system sys(root);
	graphics::camera::load(root["camera"]);
	active = std::move(sys);
}

