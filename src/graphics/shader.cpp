
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

#include "shader.hpp"
#include "window.hpp"

using namespace Sim::Graphics;

Shader Shader::MAIN;
Shader Shader::LIGHT;
Shader* Shader::ACTIVE;

static std::string read_shader(const char* path)
{
	std::stringstream ss;
	std::ifstream file(path, std::ios::binary);
	char buff[1024];

	if(!file.is_open())
	{
		throw std::runtime_error(std::format("Shader Read Error: {0}", path));
	}

	while(!file.eof())
	{
		file.read(buff, 1024);
		ss.write(buff, file.gcount());
	}

	return ss.str();
}

static std::string read_shader(const char* base, const char* file)
{
	std::string path = std::string(base) + "/" + std::string(file);
	return read_shader(path.c_str());
}

Shader::Source::Source(const char* path, GLenum type)
{
	int success;
	std::string src = read_shader(path);
	const char* c_src = src.c_str();

	id = glCreateShader(type);
	glShaderSource(id, 1, &c_src, nullptr);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::string entry = std::format("Shader Compile Error ({0}): {1}", path, infoLog);
		throw std::runtime_error(entry);
	}
}

Shader::Source::Source(Source&& o)
{
	id = o.id;
	o.id = 0;
}

Shader::Source::~Source()
{
	if(id) glDeleteShader(id);
}

Shader::Shader()
{

}

Shader::Shader(Shader&& o)
{
	prog_id = o.prog_id;
	o.prog_id = 0;
}

Shader::~Shader()
{
	if(prog_id)
	{
		glDeleteProgram(prog_id);
	}
}

void Shader::load(const Source* sources, int count)
{
	int success;
	prog_id = glCreateProgram();

	for(int i = 0; i < count; i++)
	{
		glAttachShader(prog_id, sources[i].id);
	}

	glLinkProgram(prog_id);
	glGetProgramiv(prog_id, GL_LINK_STATUS, &success);

	if(!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(prog_id, 512, NULL, infoLog);
		std::string entry = std::format("Shader Link Error: {0}", infoLog);
		throw std::runtime_error(entry);
	}
}

void Shader::use()
{
	glUseProgram(prog_id);
	ACTIVE = this;
}

void Shader::block_binding(const char* name, unsigned int binding)
{
	glUniformBlockBinding(prog_id, get_block_index(name), binding);
}

unsigned int Shader::operator [](const char* pos)
{
	auto it = uniform_locations.find(pos);

	if(it != uniform_locations.end())
	{
		return it->second;
	}

	return uniform_locations[pos] = glGetUniformLocation(prog_id, pos);
}

unsigned int Shader::get_block_index(const char* pos)
{
	auto it = uniform_block_indices.find(pos);

	if(it != uniform_block_indices.end())
	{
		return it->second;
	}

	return uniform_block_indices[pos] = glGetUniformBlockIndex(prog_id, pos);
}

unsigned int Shader::get(const char* pos)
{
	return operator[](pos);
}

