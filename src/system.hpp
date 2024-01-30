
#pragma once

#include <memory>

#include "reactor/coolant/vessel.hpp"
#include "reactor/reactor.hpp"
#include "coolant/pump.hpp"
#include "coolant/valve.hpp"
#include "graphics/mesh/mesh.hpp"

namespace sim
{

struct system
{
	std::unique_ptr<sim::reactor::reactor> reactor;
	std::unique_ptr<sim::reactor::coolant::vessel> vessel;
	std::unique_ptr<sim::coolant::valve<sim::reactor::coolant::vessel>> valve;
	std::unique_ptr<sim::coolant::pump<sim::reactor::coolant::vessel>> pump;
	sim::graphics::mesh scene;

	system();
	system(system&& o);
	system(const system& o) = delete;

	void update(double dt);
};

};

