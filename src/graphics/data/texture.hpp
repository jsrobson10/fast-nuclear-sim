
#pragma once

#include <GL/glew.h>

#include <string>
#include <glm/vec2.hpp>

namespace Sim::Graphics::Data::Texture
{

extern uint32_t handle_white;
extern uint32_t handle_normal;

void init();
void generate_atlas();
uint32_t load(std::string path);
uint32_t load_mem(const uint8_t* data, int width, int height, int channels, int* swizzleMask = nullptr);
uint32_t load_mem(const uint8_t* data, size_t len);

};

