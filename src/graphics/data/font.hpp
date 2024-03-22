
#pragma once

#include "mesh.hpp"

#include <string>
#include <sstream>

#include <glm/vec2.hpp>

namespace Sim::Graphics::Data
{

class Font
{
	struct Character
	{
		uint32_t handle;
		glm::vec2 size;
		glm::vec2 bearing;
		float advance;
	};

	float texel_size;
	Character characters[128];

public:

	Font();
	
	void init(const std::string& name, int size);

	struct load_text_t
	{
		float size = 0.04;
		glm::vec2 align = glm::vec2(0, 0);
		Arrays::Vertex prim_base = {.material={0, 0, 1}};
		glm::mat4 mat = glm::mat4(1);
		bool bake = false;
	};

	void load_text(Mesh& rmesh, const std::string& text, load_text_t t) const;

	float calc_line_width(const char* text, float size) const;
	float calc_height(const char* text, float size) const;
};

namespace Fonts
{
	void init();

	extern Font BASE;
	extern Font MONO;
}

};

