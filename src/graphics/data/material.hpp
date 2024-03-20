
#pragma once

#include <glm/vec4.hpp>

namespace Sim::Graphics::Data
{

struct Material
{
	uint32_t diffuse = Texture::handle_white;
	uint32_t normal = Texture::handle_normal;
	glm::vec4 colour = {1, 1, 1, 1};
	float roughness = 0.5;
	float metalness = 0;
	float luminance = 0;
//	float padding = 0;

/*	static void init();
	static int create(Material m);
	static void destroy(int id);
	static Material get(int id);
	static void sync();*/
};

};

