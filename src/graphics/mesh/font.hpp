
#pragma once

#include "mesh.hpp"

#include <string>

namespace sim::graphics::font
{

void init();
void generate(mesh& m, const char* text, double size);

};

