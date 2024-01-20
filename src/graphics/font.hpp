
#pragma once

namespace sim::graphics::font
{

struct character
{
	unsigned long handle;
	long advance;
	glm::ivec2 size;
	glm::ivec2 bearing;
};

void init();

extern character chars[128];

};

