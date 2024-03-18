
#pragma once

#include "light.hpp"

namespace Sim::Graphics::Data
{

struct GLLight
{
	const int size;

	unsigned int id, fbo;
	unsigned long handle;
	Light light;

	GLLight(Light light);
	GLLight(GLLight&& o);
	GLLight(const GLLight& o) = delete;
	~GLLight();
	
	static void init();

	void render();
	void render_player();
};

};

