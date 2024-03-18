
#pragma once

#include <GL/glew.h>

#include <unordered_map>

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

		Source(const char* path, GLenum type);
		Source(const Source& o) = delete;
		Source(Source&& o);
		~Source();
	};
	
	static Shader MAIN;
	static Shader LIGHT;

	static Shader* ACTIVE;

	Shader();
	Shader(const Shader& o) = delete;
	Shader(Shader&& o);
	~Shader();

	void load(const Source* sources, int count);
	void block_binding(const char* name, unsigned int index);
	void use();

	unsigned int operator[](const char* pos);
	unsigned int get_block_index(const char* pos);
	unsigned int get(const char* pos);
};

};

