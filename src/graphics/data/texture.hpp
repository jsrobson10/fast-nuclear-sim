
#pragma once

#include <GL/glew.h>

#include <string>

namespace Sim::Graphics::Data::Texture
{

extern uint64_t handle_white;
extern uint64_t handle_normal;

void init();
uint64_t load(std::string path);
uint64_t load_mem(const void* data, int width, int height, int channels);
uint64_t load_mem(const unsigned char* data, size_t len);

};

