
#pragma once

#include <vector>

#include "light.hpp"

namespace Sim::Graphics::Data
{

class GLLights
{
	int size;

	void init_fbo();

public:
	
	unsigned int texid = 0, fbo = 0, ssbo = 0;

	std::vector<Light> lights;

	GLLights(std::vector<Light>&& lights);
	GLLights(GLLights&& o);
	GLLights(const GLLights& o) = delete;
	~GLLights();
	
	static void init();

	void render();
};

};

