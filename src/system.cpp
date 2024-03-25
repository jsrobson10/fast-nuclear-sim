
#include "system.hpp"

#include <fstream>
#include <filesystem>

#include "reactor/builder.hpp"
#include "reactor/control/boron_rod.hpp"
#include "reactor/fuel/fuel_rod.hpp"
#include "reactor/coolant/pipe.hpp"
#include "reactor/coolant/heater.hpp"
#include "graphics/camera.hpp"
#include "graphics/window.hpp"
#include "graphics/input/focus.hpp"

using namespace Sim;

std::unique_ptr<Sim::System> System::active = std::make_unique<Sim::System>("");

const char* CORE_LAYOUT[] = {
	"      C C      ",
	"   C CFCFC C   ",
	"  CFCFCFCFCFC  ",
	" CFCFCFCFCFCFC ",
	"  CFCFCFCFCFC  ",
	" CFCFCFCFCFCFC ",
	"CFCFCFCFCFCFCFC",
	" CFCFCFCFCFCFC ",
	"CFCFCFCFCFCFCFC",
	" CFCFCFCFCFCFC ",
	"  CFCFCFCFCFC  ",
	" CFCFCFCFCFCFC ",
	"  CFCFCFCFCFC  ",
	"   C CFCFC C   ",
	"      C C      "
};

System::System(const std::string& path)
	: vessel(Coolant::WATER, 8, 10, 6e6, 5e5, 10)
	, reactor(Reactor::Builder(15, 15, 0.55, 4, Reactor::Fuel::FuelRod(0.5), &vessel, CORE_LAYOUT))
	, evaporator(Coolant::WATER, 2, 30, 0, 1000)
	, pool(Coolant::WATER, {16, 32, 11.3}, 16, 1e5, 16 * 32 * 11.3 * 1000)
	, sink(Coolant::WATER, 11, 0, 0)
	, grid()
	, freight_pump(&sink, &evaporator, 1e5, 1, 1e4, 0.1, 10, Coolant::Pump::mode_t::DST, 1e6)
	, loop(&vessel, &evaporator, &grid)
	, path(path)
{
	
}

System::System(const Json::Value& node, const std::string& path)
	: clock(node["clock"].asDouble())
	, vessel(node["vessel"])
	, reactor(node["reactor"], &vessel)
	, grid(node["grid"])
	, evaporator(node["evaporator"])
	, pool(node["pool"])
	, sink(evaporator.fluid, 11, 0, 0)
	, freight_pump(node["pump"]["freight"], &sink, &evaporator)
	, loop(node, &vessel, &evaporator, &grid)
	, path(path)
{
	
}

void System::update(double dt)
{
	if(!Graphics::Focus::should_advance_time())
	{
		return;
	}

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
	node["pool"] = pool;
	node["pump"]["freight"] = freight_pump;
	node["reactor"] = reactor;
	node["clock"] = clock;

	return node;
}

void System::save()
{
	// make sure the directory exists, if not create it
	std::filesystem::path dir = std::filesystem::path(active->path).parent_path();
	if(!std::filesystem::exists(dir))
	{
		std::filesystem::create_directories(dir);
	}

	Json::Value root(*active);
	root["camera"] = Graphics::Camera::serialize();

	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";

	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream savefile(active->path);
	writer->write(root, &savefile);
	savefile.close();
}

void System::load(const std::string& path)
{
	Json::Value root;
	std::ifstream savefile(path);
	savefile >> root;
	savefile.close();

	Graphics::Camera::load(root["camera"]);
	std::unique_ptr<System> sys = std::make_unique<System>(root, path);

	active = std::move(sys);
	Graphics::Window::reload();
}

