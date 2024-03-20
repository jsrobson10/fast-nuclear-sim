
#pragma once

#include <vector>

#include "light.hpp"

namespace Sim::Graphics::Data
{

struct GLLights
{
	const int size;

	unsigned int texid, fbo, ssbo;

	std::vector<Light> lights;

	GLLights(std::vector<Light>&& lights);
	GLLights(GLLights&& o);
	GLLights(const GLLights& o) = delete;
	~GLLights();
	
	static void init();

	void render();
};

};

