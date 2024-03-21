
#pragma once

#include <GL/glew.h>

#include <unordered_map>
#include <string>

namespace Sim::Graphics
{


class Shader
{
	unsigned int prog_id = 0;

	std::unordered_map<const char*, unsigned int> uniform_locations;
	std::unordered_map<const char*, unsigned int> uniform_block_indices;
	
public:

	struct Source
	{
		unsigned int id;

		Source(const char* data, const char* name, GLenum type);
		Source(const Source& o) = delete;
		Source(Source&& o);
		~Source();
	};
	
	static Shader MAIN;
	static Shader LIGHT;
	static Shader CCTV;

	static Shader* ACTIVE;

	static bool USE_BINDLESS_TEXTURES;

	Shader();
	Shader(const Shader& o) = delete;
	Shader(Shader&& o);
	~Shader();

	static void add_define(const std::string& flag);
	static void init();

	void load(const Source* sources, const char* name, int count);
	void block_binding(const char* name, unsigned int index);
	void use();

	unsigned int operator[](const char* pos);
	unsigned int get_block_index(const char* pos);
	unsigned int get(const char* pos);
};

};

