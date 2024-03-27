
#pragma once

#include "source.hpp"
#include "../graphics/data/mesh.hpp"

#include <vector>

namespace Sim::Audio::SoundEngine
{
	void update();
	void source_process(Source* source);
	void source_register(Source* source);
	void source_unregister(Source* source);
	void source_reregister(Source* source_old, Source* source_new);
	void remesh_debug(Graphics::Data::Mesh& rmesh);
};

