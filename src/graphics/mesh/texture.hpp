
#pragma once

#include <string>

namespace sim::graphics::texture
{

extern unsigned int handle_white;

void init();
unsigned int load(std::string path);
unsigned int load_mem(const unsigned char* data, int width, int height, int channels);
unsigned int load_mem(const unsigned char* data, size_t len);

};

