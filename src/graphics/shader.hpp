
#pragma once

#include <unordered_map>

namespace Sim::Graphics
{

class Shader
{
	unsigned int prog_id = 0;

	std::unordered_map<const char*, unsigned int> uniform_locations;
	std::unordered_map<const char*, unsigned int> uniform_block_indices;
	
public:
	
	static Shader MAIN;
	static Shader BLUR;
	static Shader LIGHT;

	static Shader* ACTIVE;

	Shader();
	Shader(const Shader& o) = delete;
	Shader(Shader&& o);
	~Shader();

	void load(const char* path, const char* file_vsh, const char* file_gsh, const char* file_fsh);
	void load(const char* path, const char* file_vsh, const char* file_fsh);
	void block_binding(const char* name, unsigned int index);
	void use();

	unsigned int operator[](const char* pos);
	unsigned int get_block_index(const char* pos);
	unsigned int get(const char* pos);
};

};

