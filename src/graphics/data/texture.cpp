
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <stb/stb_rect_pack.h>

#include <glm/matrix.hpp>

#include <unordered_map>
#include <iostream>
#include <cmath>

#include "texture.hpp"
#include "atlas.hpp"
#include "../shader.hpp"

using namespace Sim::Graphics::Data;

static bool is_done = false;
static uint32_t atlas_texid;
static uint32_t atlas_uv_ssbo;
static std::unordered_map<std::string, uint32_t> loaded;
static std::vector<Atlas<4>> texture_atlas_queue;

uint32_t Texture::handle_white;
uint32_t Texture::handle_normal;

void Texture::init()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char pixels_white[] = {255};
	unsigned char pixels_normal[] = {128, 255};
	int swizzle_white[] = {GL_RED, GL_RED, GL_RED, GL_RED};
	int swizzle_normal[] = {GL_RED, GL_RED, GL_GREEN, GL_GREEN};
	handle_white = load_mem(pixels_white, 1, 1, 1, swizzle_white);
	handle_normal = load_mem(pixels_normal, 1, 1, 2, swizzle_normal);
}

void Texture::generate_atlas()
{
	// if we are using bindless textures, we don't need to generate an atlas
	if(Shader::USE_BINDLESS_TEXTURES)
	{
		return;
	}

	int total_area = 0;
	int padding = 2;
	int offset = 1;

	for(const Atlas<4>& atlas : texture_atlas_queue)
	{
		total_area += (atlas.width + padding) * (atlas.height + padding);
	}

	int size = std::pow(2, std::ceil(std::log2(std::sqrt(total_area))));

	std::vector<stbrp_rect> rects;
	std::vector<glm::mat2> uvs;

	rects.reserve(texture_atlas_queue.size());
	uvs.reserve(texture_atlas_queue.size());

	for(int i = 0; i < texture_atlas_queue.size(); i++)
	{
		const Atlas<4>& atlas = texture_atlas_queue[i];
		stbrp_rect rect;
		rect.id = i;
		rect.w = atlas.width + padding;
		rect.h = atlas.height + padding;
		rects.push_back(rect);
	}

	stbrp_context context;
	std::vector<stbrp_node> nodes(size);

	for(;;)
	{
		stbrp_init_target(&context, size, size, nodes.data(), nodes.size());

		if(stbrp_pack_rects(&context, rects.data(), rects.size()) == 1)
		{
			break;
		}

		size *= 2;
		nodes.resize(size);
		std::cout << "Error: failed to pack textures, trying again with size " << size << "\n";
	}

	Atlas<4> atlas(size, size);

	for(const stbrp_rect& rect : rects)
	{
		const Atlas<4>& src = texture_atlas_queue[rect.id];
		atlas.draw(src, rect.x + offset, rect.y + offset, true);

		uvs.emplace_back(glm::mat2(
			(rect.x + offset + 0.5f) / size, (rect.y + offset + 0.5f) / size,
			(rect.x + offset + src.width - 0.5f) / size, (rect.y + offset + src.height - 0.5f) / size
		));
	}

	std::cout << "Finished stitching " << size << "x" << size << " texture atlas\n";

	glGenTextures(1, &atlas_texid);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, atlas_texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glGenBuffers(1, &atlas_uv_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, atlas_uv_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, uvs.size() * sizeof(uvs[0]), uvs.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, atlas_uv_ssbo);
	
	glUniform1i(Shader::MAIN["tex_atlas"], 1);

	is_done = true;
}

uint32_t Texture::load_mem(const uint8_t* data, int width, int height, int channels, int* swizzleMask)
{
	if(is_done)
	{
		throw std::runtime_error("Texture loading is done");
	}

	if(!data)
	{
		return 0;
	}

	int swizzleMaskDefault[] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};

	if(!swizzleMask)
	{
		swizzleMask = swizzleMaskDefault;

		switch(channels)
		{
		case 1:
			swizzleMask[1] = GL_ONE;
		case 2:
			swizzleMask[2] = GL_ONE;
		case 3:
			swizzleMask[3] = GL_ONE;
		}
	}

	if(!Shader::USE_BINDLESS_TEXTURES)
	{
		Atlas<4> atlas(width, height);

		for(int i = 0; i < width * height; i++)
		{
			int pixel_pos = i * channels;
			Atlas<4>::Pixel pixel;

			for(int j = 0; j < 4; j++)
			{
				switch(swizzleMask[j])
				{
				case GL_RED:
					pixel[j] = data[pixel_pos];
					break;
				case GL_GREEN:
					pixel[j] = data[pixel_pos + 1];
					break;
				case GL_BLUE:
					pixel[j] = data[pixel_pos + 2];
					break;
				case GL_ALPHA:
					pixel[j] = data[pixel_pos + 3];
					break;
				case GL_ZERO:
					pixel[j] = 0;
					break;
				case GL_ONE:
					pixel[j] = 255;
					break;
				}
			}

			atlas.data[i] = pixel;
		}
		
		texture_atlas_queue.push_back(std::move(atlas));
		return texture_atlas_queue.size() - 1;
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

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, format_in, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteriv(texid, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
	glGenerateMipmap(GL_TEXTURE_2D);

	uint32_t handle = glGetTextureHandleARB(texid);
	glMakeTextureHandleResidentARB(handle);
	return handle;
}

uint32_t Texture::load_mem(const uint8_t* filedata, size_t len)
{
	int width, height, channels;
	unsigned char* data = stbi_load_from_memory(filedata, len, &width, &height, &channels, 0);
	uint64_t handle = load_mem(data, width, height, channels);
	stbi_image_free(data);
	return handle;
}

uint32_t Texture::load(std::string path)
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

