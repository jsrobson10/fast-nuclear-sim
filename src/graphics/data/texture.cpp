
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <unordered_map>
#include <iostream>

#include "texture.hpp"

using namespace Sim::Graphics::Data;

static std::unordered_map<std::string, uint64_t> loaded;
uint64_t Texture::handle_white;
uint64_t Texture::handle_normal;

void Texture::init()
{
	unsigned char pixels_white[] = {255, 255, 255};
	unsigned char pixels_normal[] = {128, 128, 255};
	handle_white = load_mem(pixels_white, 1, 1, 3);
	handle_normal = load_mem(pixels_normal, 1, 1, 3);
}

uint64_t Texture::load_mem(const void* data, int width, int height, int channels)
{
	if(!data)
	{
		return 0;
	}

	GLenum format;
	GLenum format_in;

	switch(channels)
	{
	case 1:
		format = GL_RED;
		format_in = GL_R8;
		break;
	case 2:
		format = GL_RG;
		format_in = GL_RG8;
		break;
	case 3:
		format = GL_RGB;
		format_in = GL_RGB8;
		break;
	case 4:
		format = GL_RGBA;
		format_in = GL_RGBA8;
		break;
	default:
		throw std::runtime_error("Invalid number of channels: " + std::to_string(channels));
	}

	unsigned int texid;

	glCreateTextures(GL_TEXTURE_2D, 1, &texid);
	glTextureStorage2D(texid, 1, format_in, width, height);
	glTextureSubImage2D(texid, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

	glTextureParameteri(texid, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texid, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(texid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texid, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateTextureMipmap(texid);

	uint64_t handle = glGetTextureHandleARB(texid);
	glMakeTextureHandleResidentARB(handle);
	return handle;
}

uint64_t Texture::load_mem(const unsigned char* filedata, size_t len)
{
	int width, height, channels;
	unsigned char* data = stbi_load_from_memory(filedata, len, &width, &height, &channels, 0);
	uint64_t handle = load_mem(data, width, height, channels);
	stbi_image_free(data);
	return handle;
}

uint64_t Texture::load(std::string path)
{
	const auto it = loaded.find(path);

	if(it != loaded.end())
	{
		return it->second;
	}

	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	uint64_t handle = load_mem(data, width, height, channels);
	stbi_image_free(data);

	if(handle == 0)
	{
		throw std::runtime_error("Failed to load path: " + path);
	}

	std::cout << "Loaded image: " << path << "\n";

	loaded[path] = handle;
	return handle;
}

