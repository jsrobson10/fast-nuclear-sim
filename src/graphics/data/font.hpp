
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
	Mesh load_text(const std::string& text, float size, glm::vec2 align) const;
	Mesh load_text(const std::string& text, float size) const;

	template <class T>
	void load_text(const char* header, T& item, double size)
	{
		std::stringstream ss;
		ss << header << item;
		load_text(ss.str(), size);
	}
	
	template <class T>
	void load_text(const char* header, T& item, double size, glm::vec2 align)
	{
		std::stringstream ss;
		ss << header << item;
		load_text(ss.str(), size, align);
	}
};

namespace Fonts
{
	void init();

	extern Font BASE;
	extern Font MONO;
}

};

