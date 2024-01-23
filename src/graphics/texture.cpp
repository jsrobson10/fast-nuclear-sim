
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <unordered_map>
#include <iostream>

#include "texture.hpp"

using namespace sim::graphics;

static std::unordered_map<std::string, unsigned int> loaded; 

unsigned int texture::load(std::string path)
{
	const auto it = loaded.find(path);

	if(it != loaded.end())
	{
		return it->second;
	}

	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if(!data)
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load path: " + path);
	}

	GLenum format, format_in;
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
	}
	
	unsigned int texid;

	glCreateTextures(GL_TEXTURE_2D, 1, &texid);
	glTextureStorage2D(texid, 1, format_in, width, height);
	glTextureSubImage2D(texid, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glTextureParameteri(texid, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texid, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texid, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texid, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateTextureMipmap(texid);

	unsigned int handle = glGetTextureHandleARB(texid);
	glMakeTextureHandleResidentARB(handle);

	std::cout << "Loaded Image: " << path << "\n";

	loaded[path] = handle;
	return handle;
}

