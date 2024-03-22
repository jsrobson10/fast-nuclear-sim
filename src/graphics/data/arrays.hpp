
#pragma once

#include "texture.hpp"

#include <glm/matrix.hpp>
#include <ostream>

namespace Sim::Graphics::Data::Arrays
{

struct Vertex
{
	glm::vec2 texpos = {0, 0};
	glm::vec3 pos = {0, 0, 0};
	glm::vec4 colour = {1, 1, 1, 1};
	glm::mat3 tbn = glm::mat3(1);
	int transform_id = -1;
	int colour_id = -1;
	unsigned int tex_diffuse = Texture::handle_white;
	unsigned int tex_normal = Texture::handle_normal;
	glm::vec3 material = {0.5, 0, 0};

	constexpr bool operator==(const Vertex&) const = default;

	friend std::ostream& operator<<(std::ostream& os, const Vertex& v);

};

void vertex_attrib_pointers();

};

