
#pragma once

#include "mesh.hpp"

#include <string>
#include <sstream>

namespace sim::graphics::font
{

void init();
void generate(mesh& m, const char* text, double size);

template <class T>
void generate(mesh& m, const char* header, T* item, double size)
{
	std::stringstream ss;
	ss << header << *item;
	generate(m, ss.str().c_str(), size);
}

};

