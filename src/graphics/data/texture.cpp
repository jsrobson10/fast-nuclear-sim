
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

struct AtlasPart
{
	glm::vec2 uv0;
	glm::vec2 uv1;
	int zpos;
	int edges; // 0 = repeat, 1 = clamp
	int padding[2];
};

struct AtlasQueueItem
{
	Atlas<4> atlas;
	int edges;
};

static bool is_done = false;
static uint32_t atlas_texid;
static uint32_t atlas_uv_ssbo;
static std::unordered_map<std::string, uint32_t> loaded;
static std::vector<AtlasQueueItem> texture_atlas_queue;

uint32_t Texture::handle_white;
uint32_t Texture::handle_normal;

void Texture::init()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char pixels_white[] = {255};
	unsigned char pixels_normal[] = {128, 128, 255};
	int swizzle_white[] = {GL_RED, GL_RED, GL_RED, GL_RED};
	handle_white = load_mem(pixels_white, 1, 1, 1, swizzle_white);
	handle_normal = load_mem(pixels_normal, 1, 1, 3);
}

void Texture::generate_atlas()
{
	// if we are using bindless textures, we don't need to generate an atlas
	if(Shader::USE_BINDLESS_TEXTURES)
	{
		return;
	}

	int total_area = 0;
	int min_size = 1;
	int padding = 0;
	int offset = 0;

	for(const AtlasQueueItem& a : texture_atlas_queue)
	{
		int w = a.atlas.width + padding;
		int h = a.atlas.height + padding;
		if(w > min_size) min_size = w;
		if(h > min_size) min_size = h;
		total_area += w * h;
	}

	// find a good approximate size for the atlas that can hold all the textures
	int size = std::pow(2, std::ceil(std::log2(std::max((double)min_size, std::pow(total_area, 1.0/3.0)))));

	std::vector<stbrp_rect> rects;
	std::vector<AtlasPart> uvs(texture_atlas_queue.size());
	rects.reserve(texture_atlas_queue.size());

	for(int i = 0; i < texture_atlas_queue.size(); i++)
	{
		const AtlasQueueItem& a = texture_atlas_queue[i];
		stbrp_rect rect;
		rect.id = i;
		rect.w = a.atlas.width + padding;
		rect.h = a.atlas.height + padding;
		rects.push_back(rect);
	}

	stbrp_context context;
	std::vector<stbrp_node> nodes(size);
	Atlas<4> atlas(size, size);
	int zpos;

	for(zpos = 0; rects.size() > 0; zpos++)
	{
		stbrp_init_target(&context, size, size, nodes.data(), nodes.size());
		stbrp_pack_rects(&context, rects.data(), rects.size());

		for(auto it = rects.begin(); it != rects.end();)
		{
			if(!it->was_packed)
			{
				it++;
				continue;
			}
			
			const AtlasQueueItem& src = texture_atlas_queue[it->id];
			atlas.draw(src.atlas, it->x + offset, it->y + offset, zpos, false);

			uvs[it->id] = {
				{(it->x + offset + 0.5f) / size, (it->y + offset + 0.5f) / size},
				{(it->x + offset + src.atlas.width - 0.5f) / size, (it->y + offset + src.atlas.height - 0.5f) / size},
				zpos, src.edges
			};

			it = rects.erase(it);
		}
	}

	std::cout << "Finished stitching " << size << "x" << size << "x" << zpos << " texture atlas\n";

	glGenTextures(1, &atlas_texid);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, atlas_texid);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, size, size, zpos, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas.data.data());
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	glActiveTexture(GL_TEXTURE0);

	glGenBuffers(1, &atlas_uv_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, atlas_uv_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, uvs.size() * sizeof(uvs[0]), uvs.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, atlas_uv_ssbo);
	
	glUniform1i(Shader::MAIN["tex_atlas"], 1);

	is_done = true;
}

uint32_t Texture::load_mem(const uint8_t* data, int width, int height, int channels, int* swizzleMask, GLenum edgeBehaviour)
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
		
		texture_atlas_queue.push_back({
			std::move(atlas),
			edgeBehaviour == GL_CLAMP_TO_EDGE ? 1 : 0
		});

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edgeBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

