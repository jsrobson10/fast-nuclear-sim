
#include "system.hpp"

#include <fstream>

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "graphics/camera.hpp"
#include "graphics/window.hpp"

using namespace Sim;

std::unique_ptr<Sim::System> System::active = std::make_unique<Sim::System>();

const char* CORE_LAYOUT[] = {
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

System::System() :
		vessel(Coolant::WATER, 8, 10, 6e6, 5e5, 10),
		reactor(Reactor::Builder(19, 19, 0.4, 4, Reactor::Fuel::FuelRod(0.5), &vessel, CORE_LAYOUT)),
		evaporator(Coolant::WATER, 2, 30, 0, 1000),
		sink(Coolant::WATER, 11, 0, 0),
		grid(),
		freight_pump(&sink, &evaporator, 1e5, 1, 1e4, 0.1, 10, Coolant::Pump::mode_t::DST, 1e6),
		loop(&vessel, &evaporator, &grid)
{
	
}

System::System(const Json::Value& node) :
		clock(node["clock"].asDouble()),
		vessel(node["vessel"]),
		reactor(node["reactor"], &vessel),
		grid(node["grid"]),
		evaporator(node["evaporator"]),
		sink(evaporator.fluid, 11, 0, 0),
		freight_pump(node["pump"]["freight"], &sink, &evaporator),
		loop(node, &vessel, &evaporator, &grid)
{
	
}

void System::update(double dt)
{
	dt *= speed;
	clock += dt;
	
	grid.update(dt);
	reactor.update(dt);
	vessel.update(dt);
	freight_pump.update(dt);
	evaporator.update(dt);
	loop.update(dt);
}

System::operator Json::Value() const
{
	Json::Value node(loop);

	node["grid"] = grid;
	node["vessel"] = vessel;
	node["evaporator"] = evaporator;
	node["pump"]["freight"] = freight_pump;
	node["reactor"] = reactor;
	node["clock"] = clock;

	return node;
}

void System::save(const char* path)
{
	Json::Value root(*active);
	root["camera"] = Graphics::Camera::serialize();

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream savefile(path);
	writer->write(root, &savefile);
	savefile.close();
}

void System::load(const char* path)
{
	Json::Value root;
	std::ifstream savefile(path);
	savefile >> root;
	savefile.close();

	Graphics::Camera::load(root["camera"]);
	std::unique_ptr<System> sys = std::make_unique<System>(root);
	active = std::move(sys);

	Graphics::Window::reload();
}

void System::save()
{
	save("savefile.json");
}

void System::load()
{
	load("savefile.json");
}

